#include "mainwindow.h"
#include "defaultpuzzlemenuwindow.h"
#include "generatepuzzlemenuwindow.h"
#include <QVBoxLayout>
#include <QPushButton>
#include "puzzle/pieceslist.h"
#include "puzzle/puzzlewidget.h"
#include "bmp/bmp.h"

#include <QtWidgets>
#include <stdlib.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Puzzle Game"));

    // ✅ Buat central widget
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // ✅ Layout vertical untuk tombol
    QVBoxLayout *layout = new QVBoxLayout(central);

    // ✅ Tombol menu
    QPushButton *defaultPuzzleBtn = new QPushButton("Puzzle Bawaan");
    QPushButton *generatePuzzleBtn = new QPushButton("Generate Puzzle");

    layout->addWidget(defaultPuzzleBtn);
    layout->addWidget(generatePuzzleBtn);

    // ✅ Connect tombol ke slot
    connect(defaultPuzzleBtn, &QPushButton::clicked, this, &MainWindow::openDefaultPuzzleMenu);
    connect(generatePuzzleBtn, &QPushButton::clicked, this, &MainWindow::openGeneratePuzzleMenu);
}

void MainWindow::openDefaultPuzzleMenu()
{
    auto *win = new DefaultPuzzleMenuWindow;
    win->show();
}

void MainWindow::openGeneratePuzzleMenu()
{
    auto *win = new GeneratePuzzleMenuWindow;
    win->show();
}
