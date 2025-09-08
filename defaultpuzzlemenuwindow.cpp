#include "defaultpuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QPushButton>

DefaultPuzzleMenuWindow::DefaultPuzzleMenuWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Pilih Level Puzzle");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *btn3x3 = new QPushButton("Level 3x3");
    QPushButton *btn5x5 = new QPushButton("Level 5x5");
    QPushButton *btn7x7 = new QPushButton("Level 7x7");

    layout->addWidget(btn3x3);
    layout->addWidget(btn5x5);
    layout->addWidget(btn7x7);

    connect(btn3x3, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle3x3);
    connect(btn5x5, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle5x5);
    connect(btn7x7, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle7x7);
}

void DefaultPuzzleMenuWindow::startPuzzle3x3()
{
    MainWindowPuzzle *win = new MainWindowPuzzle(":/images/colorImg.bmp", ":/images/greyImg.bmp", 3, 300);
    win->show();
}

void DefaultPuzzleMenuWindow::startPuzzle5x5()
{
    MainWindowPuzzle *win = new MainWindowPuzzle(":/images/colorImg.bmp", ":/images/greyImg.bmp", 5, 300);
    win->show();
}

void DefaultPuzzleMenuWindow::startPuzzle7x7()
{
    MainWindowPuzzle *win = new MainWindowPuzzle(":/images/colorImg.bmp", ":/images/greyImg.bmp", 7, 300);
    win->show();
}

