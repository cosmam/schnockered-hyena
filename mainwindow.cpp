#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"

#include <QThread>

#include <QtConcurrent/QtConcurrent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_board( NULL ),
    m_updateTimeout(1000)
{
    ui->setupUi(this);

    connect( ui->btn_Start, SIGNAL(clicked()),
             this, SLOT(StartClicked()) );

    m_updateTimer.setInterval(m_updateTimeout);

    connect( &m_updateTimer, SIGNAL( timeout() ),
             this, SLOT( UpdateTimerTimeout() ) );

    ui->threads_SpinBox->setMaximum( QThread::idealThreadCount() - 1 );

    ui->solutions_Widget->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ConstructBoard( uint rows, uint cols )
{
    if( m_board )
    {
        delete m_board;
        m_board = NULL;
    }

    m_board = new Board( rows, cols );
}

void MainWindow::StartClicked()
{
    m_paths.clear();

    m_updates = 0;
    m_updateTimer.start();

    ui->calc_ProgressBar->setMaximum(0);
    ui->progress_Label->setText("Calculating...");
    SetTimeText( 0.0 );
    SetSolutionsVisibility(false);
    ui->solutions_Widget->show();

    uint rows = ui->row_SpinBox->value();
    ConstructBoard( rows, ui->col_SpinBox->value() );

    Path::SetBoard( m_board );
    Path::SetStop( m_board->GetSquare( rows - 1, 0 ) );
    Path::SetTargetSize( m_board->GetSize() );
    Path::SetMaxRow( m_board->GetMaxRow() );
    Path::SetMaxCol( m_board->GetMaxCol() );
    Path::SetEndContitions( ui->btn_EndConditions->isChecked() );

    QList<Path> paths;
    Path path( m_board->GetSquare(0,0) );
    path.SetBoard( m_board );
    paths.append( path );

    Refresh( ui->DrawingTab );

    if( m_board->GetSize() < 25 )
    {
        m_timer.start();

        m_paths = CalculateSmall( paths );

        Finish();
    }
    else
    {
        Calculate( paths );
    }
}

QList<Path> MainWindow::CalculateSmall( const QList<Path> paths )
{
    QList<Path> retPaths = paths;

    bool allFound(false);
    while( !allFound )
    {
        retPaths = IncrementPaths( retPaths, allFound );
    }

    return retPaths;
}

QList<Path> MainWindow::Calculate( const QList<Path> paths )
{   
    m_timer.start();

    QList<Path> retPaths = paths;

    int threadCount = ui->threads_SpinBox->value();

    bool allFound(false);
    while( !allFound && (retPaths.size() < threadCount) )
    {
        retPaths = IncrementPaths( retPaths, allFound );
    }

    QVector< QList<Path> > allPaths;
    QList<Path> path;
    allPaths.fill( path, threadCount );

    int i=0;
    while( !retPaths.isEmpty() )
    {
        path = allPaths.at(i);
        path.append( retPaths.takeFirst() );
        allPaths.replace(i,path);
        ++i;

        if( i == threadCount )
        {
            i=0;
        }
    }

    m_threadsFinished = 0;

    QFutureWatcher< QList<Path> > * watcher;

    foreach( QList<Path> pathList, allPaths )
    {
        watcher = new QFutureWatcher< QList<Path> >;
        QFuture< QList<Path> > future;

        m_watcherList.append( watcher );

        connect( watcher, SIGNAL( finished() ),
                 this, SLOT( HandleFinished() ) );

        future = QtConcurrent::run( this, &MainWindow::CalculateSmall, pathList );
        watcher->setFuture( future );
    }

    return retPaths;
}

void MainWindow::Finish()
{   
    m_milliseconds = m_timer.elapsed();

    m_updateTimer.stop();

    ui->calc_ProgressBar->setMaximum(100);
    ui->calc_ProgressBar->setValue(100);
    ui->progress_Label->setText("Done!");

    ui->btn_EndConditions->setChecked(true);

    double seconds = (static_cast<double>(m_milliseconds))/1000.0;

    QString units = ( m_paths.size() == 1 ? "solution" : "solutions" );
    ui->solutions_Label->setText( QString::number( m_paths.size() ) );
    ui->solutionsUnit_Label->setText( units );

    SetTimeText( seconds );

    SetSolutionsVisibility( true );
}

QList<Path> MainWindow::IncrementPaths( QList<Path> paths, bool & allFound )
{
    QList<Path> allPaths;

    foreach( Path path, paths )
    {
        QList<Path> retPaths = path.Step();

        foreach( Path subpath, retPaths )
        {
            if( subpath.IsValid() )
            {
                allPaths.append( subpath );
            }
        }
    }

    allFound = allPaths.isEmpty();

    if( allFound )
    {
        foreach( Path path, paths )
        {
            if( path.IsValid() && ( path.Size() == m_board->GetSize() ) )
            {
                allPaths.append( path );
            }
        }
    }

    return allPaths;
}

void MainWindow::FinalPare()
{
    QList<Path> paths;

    foreach( Path path, m_paths )
    {
        if( path.IsValid() && ( path.Size() == m_board->GetSize() ) )
        {
            paths.append( path );
        }
    }

    m_paths = paths;
}

void MainWindow::Refresh(QWidget *widget)
{
    widget->style()->unpolish( widget );
    widget->style()->polish( widget );
}

void MainWindow::HandleFinished()
{
    QMutexLocker locker( &m_finishedMutex );

    m_threadsFinished++;

    if( m_threadsFinished == m_watcherList.size() )
    {
        AllFinished();
    }
}

void MainWindow::AllFinished()
{
    foreach( QFutureWatcher< QList<Path> > * watcher, m_watcherList )
    {
        foreach( QList<Path> paths, watcher->future().results() )
        {
            m_paths.append( paths );
        }

        delete watcher;
    }

    m_watcherList.clear();

    Finish();
}

void MainWindow::UpdateTimerTimeout()
{
    m_updates++;

    double seconds = static_cast<double>(m_updateTimeout * m_updates)/1000.0;

    SetTimeText( seconds );
}

void MainWindow::SetSolutionsVisibility( bool visible )
{
    ui->solutions_Label->setVisible( visible );
    ui->solutionsUnit_Label->setVisible( visible );
    ui->solutionsTag_Label->setVisible( visible );
}

void MainWindow::SetTimeText( double value )
{
    ui->time_Label->setText( QString::number( value, 'f', 3 ) );

}
