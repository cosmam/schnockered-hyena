#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Common.h"

#include <QMainWindow>
#include <QList>

class Board;
class Path;

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

    Ui::MainWindow *ui;
    Board * m_board;
    QList<Path> m_paths;
    int m_milliseconds;

private slots:

    void StartClicked();

    bool IncrementPaths( QList<Path> & paths );

    void CalculateSmall();

    void Calculate();

    void Finish();
};

#endif // MAINWINDOW_H
