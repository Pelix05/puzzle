#include "defaultpuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QPushButton>

DefaultPuzzleMenuWindow::DefaultPuzzleMenuWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Choose level you want to play");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *btn3x3 = new QPushButton("Level 1");
    QPushButton *btn5x5 = new QPushButton("Level 2");
    QPushButton *btn7x7 = new QPushButton("Level 3");
    QPushButton *backBtn = new QPushButton("Back");



    layout->addWidget(btn3x3);
    layout->addWidget(btn5x5);
    layout->addWidget(btn7x7);
    layout->addWidget(backBtn);

    connect(btn3x3, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle3x3);
    connect(btn5x5, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle5x5);
    connect(btn7x7, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle7x7);
    connect(backBtn, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::close);

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

