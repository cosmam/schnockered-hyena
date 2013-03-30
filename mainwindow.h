#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Common.h"
#include "path.h"

#include <QFuture>
#include <QFutureWatcher>
#include <QMainWindow>
#include <QMutex>
#include <QList>
#include <QTime>
#include <QTimer>

class Board;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:

    void ConstructBoard( uint rows, uint cols );

    void FinalPare();

    void Refresh( QWidget * widget );

    QList<Path> CalculateSmall( const QList<Path> paths );

    QList<Path> Calculate( const QList<Path> paths );

    Ui::MainWindow *ui;
    Board * m_board;
    QList<Path> m_paths;
    int m_milliseconds, m_threadsFinished, m_updateTimeout, m_updates;

    QMutex m_finishedMutex;

    QVector< QFutureWatcher< QList<Path> > * > m_watcherList;

    QTime m_timer;

    QTimer m_updateTimer;

private slots:

    void HandleFinished();

    void AllFinished();

    void StartClicked();

    void UpdateTimerTimeout();

    QList<Path> IncrementPaths( QList<Path> paths, bool & allFound );

    void Finish();

    void SetSolutionsVisibility( bool visible );

    void SetTimeText( double value );
};

#endif // MAINWINDOW_H
