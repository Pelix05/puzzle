#include "mainwindow.h"
#include "defaultpuzzlemenuwindow.h"
#include "generatepuzzlemenuwindow.h"
#include "leaderboard.h"
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

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *layout = new QVBoxLayout(central);

    QPushButton *defaultPuzzleBtn = new QPushButton("Puzzle Bawaan");
    QPushButton *generatePuzzleBtn = new QPushButton("Generate Puzzle");
    QPushButton *leaderboardBtn = new QPushButton("leaderboard");

    layout->addWidget(defaultPuzzleBtn);
    layout->addWidget(generatePuzzleBtn);
    layout->addWidget(leaderboardBtn);

    connect(defaultPuzzleBtn, &QPushButton::clicked, this, &MainWindow::openDefaultPuzzleMenu);
    connect(generatePuzzleBtn, &QPushButton::clicked, this, &MainWindow::openGeneratePuzzleMenu);

    // 初始化数据库和排行榜
    Database *db = new Database();
    db->initDatabase();
    DatabaseManager *dbManager = new DatabaseManager(db);

    Leaderboard *leaderboard = new Leaderboard(dbManager);
    leaderboard->setWindowTitle("排行榜");
    leaderboard->resize(500, 400);

    connect(leaderboardBtn, &QPushButton::clicked, [=]() {
        leaderboard->refresh(10); // 刷新 Top10
        leaderboard->show();      // 显示排行榜窗口
    });
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
