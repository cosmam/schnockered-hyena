#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "board.h"
#include "path.h"

#include <QThread>
#include <QTime>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_board( NULL )
{
    ui->setupUi(this);

    connect( ui->btn_Start, SIGNAL(clicked()),
             this, SLOT(StartClicked()) );

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
    ui->calc_ProgressBar->setMaximum(0);
    ui->progress_Label->setText("Calculating...");
    ui->solutions_Widget->hide();

    uint rows = ui->row_SpinBox->value();
    ConstructBoard( rows, ui->col_SpinBox->value() );

    Path path( m_board->GetSquare(0,0),
               m_board->GetSquare(rows-1,0),
               m_board->GetSize() );
    m_paths.append( path );

    Refresh( ui->DrawingTab );

    if( true || m_board->GetSize() < 30 )
    {
        QTimer::singleShot(0,this, SLOT(CalculateSmall()));
    }
    else
    {
        QTimer::singleShot(0,this, SLOT(Calculate()));
    }
}

void MainWindow::CalculateSmall()
{
    QTime myTimer;
    myTimer.start();

    bool allFound(false);
    while( !allFound )
    {
        allFound = IncrementPaths( m_paths );
    }

    FinalPare();

    m_milliseconds = myTimer.elapsed();

    QTimer::singleShot(0,this, SLOT(Finish()));
}

void MainWindow::Calculate()
{
    QTime myTimer;
    myTimer.start();

    bool allFound(false);
    while( !allFound && ( m_paths.size() < QThread::idealThreadCount() ) )
    {
        allFound = IncrementPaths( m_paths );
    }

    FinalPare();

    m_milliseconds = myTimer.elapsed();

    QTimer::singleShot(0,this, SLOT(Finish()));
}

void MainWindow::Finish()
{
    ui->calc_ProgressBar->setMaximum(100);
    ui->calc_ProgressBar->setValue(100);
    ui->progress_Label->setText("Done!");

    double seconds = (static_cast<double>(m_milliseconds))/1000.0;

    QString units = ( m_paths.size() == 1 ? "solution" : "solutions" );
    ui->solutions_Label->setText( QString::number( m_paths.size() ) );
    ui->solutionsUnit_Label->setText( units );

    ui->time_Label->setText( QString::number( seconds, 'f', 3 ) );

    ui->solutions_Widget->show();
}

bool MainWindow::IncrementPaths( QList<Path> & paths )
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

    if( !allPaths.isEmpty() )
    {
        paths = allPaths;
    }

    return allPaths.isEmpty();
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
