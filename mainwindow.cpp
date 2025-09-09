// mainwindow.cpp
#include "mainwindow.h"
#include "defaultpuzzlemenuwindow.h"
#include "generatepuzzlemenuwindow.h"
#include "databasemanager.h"
#include "leaderboard.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>
#include <QFont>
#include <QStyle>

// Implement the static background function
void MainWindow::applyBackground(QWidget *widget, const QString &imagePath)
{
    widget->setAutoFillBackground(true);
    widget->setStyleSheet(QString(
                              "QWidget {"
                              "background-image: url(%1);"
                              "background-position: center;"
                              "background-repeat: no-repeat;"
                              "}"
                              ).arg(imagePath));
}


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Puzzle Game"));
    setFixedSize(800, 600);

    // Apply background
    applyBackground(this,":/images/background.jpg");

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Semi-transparent overlay
    QFrame *overlay = new QFrame();
    overlay->setStyleSheet("QFrame {"
                           "background: rgba(0, 0, 0, 150);"
                           "border: none;"
                           "}");

    QVBoxLayout *overlayLayout = new QVBoxLayout(overlay);
    overlayLayout->setSpacing(30);
    overlayLayout->setContentsMargins(50, 50, 50, 50);

    // Title
    QLabel *titleLabel = new QLabel("PUZZLE GAME");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: white;"
                              "font-size: 36px;"
                              "font-weight: bold;"
                              "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
                              "}");
    overlayLayout->addStretch();
    overlayLayout->addWidget(titleLabel);

    // Buttons
    QPushButton *defaultPuzzleBtn = createGlassButton("Puzzle Bawaan");
    QPushButton *generatePuzzleBtn = createGlassButton("Generate Puzzle");
    QPushButton *leaderboardBtn = createGlassButton("leaderboard");

    overlayLayout->addWidget(defaultPuzzleBtn);
    overlayLayout->addWidget(generatePuzzleBtn);
    overlayLayout->addWidget(leaderboardBtn);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlay);

    connect(defaultPuzzleBtn, &QPushButton::clicked, this, &MainWindow::openDefaultPuzzleMenu);
    connect(generatePuzzleBtn, &QPushButton::clicked, this, &MainWindow::openGeneratePuzzleMenu);

    // 初始化数据库和排行榜
    Database *db = new Database();
    db->initDatabase();
    dbManager = new DatabaseManager(db);

    Leaderboard *leaderboard = new Leaderboard(dbManager);
    leaderboard->setWindowTitle("排行榜");
    leaderboard->resize(500, 400);

    connect(leaderboardBtn, &QPushButton::clicked, [=]() {
        leaderboard->refresh(10); // 刷新 Top10
        leaderboard->show();      // 显示排行榜窗口
    });
}

QPushButton* MainWindow::createGlassButton(const QString &text)
{
    QPushButton *button = new QPushButton(text);
    button->setFixedSize(250, 60);
    button->setStyleSheet(
        "QPushButton {"
        "background: rgba(255, 255, 255, 100);"
        "border: 2px solid rgba(255, 255, 255, 150);"
        "border-radius: 15px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "background: rgba(255, 255, 255, 150);"
        "border: 2px solid rgba(255, 255, 255, 200);"
        "}"
        "QPushButton:pressed {"
        "background: rgba(255, 255, 255, 200);"
        "}"
        );
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void MainWindow::openDefaultPuzzleMenu()
{
    auto *win = new DefaultPuzzleMenuWindow(dbManager);
    win->setAttribute(Qt::WA_DeleteOnClose);
    MainWindow::applyBackground(win, ":/images/background.jpg"); // Apply same background
    win->show();
}

void MainWindow::openGeneratePuzzleMenu()
{
    auto *win = new GeneratePuzzleMenuWindow(dbManager);
    win->setAttribute(Qt::WA_DeleteOnClose);
    MainWindow::applyBackground(win); // Apply same background
    win->show();
}
