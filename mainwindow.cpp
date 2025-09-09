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

void MainWindow::applyBackground(QWidget *widget, const QString &imagePath)
{
<<<<<<< HEAD
    Q_UNUSED(imagePath); // Add this line to avoid unused parameter warning

    widget->setStyleSheet(
        "QWidget {"
        "background-image: url(:/images/background.png);"
        "background-position: center;"
        "background-repeat: no-repeat;"
        "background-size: cover;"
        "}"
        );
}
// mainwindow.cpp - in the constructor
=======
    widget->setAutoFillBackground(true);
    widget->setStyleSheet(QString(
                              "QWidget {"
                              "background-image: url(%1);"
                              "background-position: center;"
                              "background-repeat: no-repeat;"
                              "}"
                              ).arg(imagePath));
}


>>>>>>> 2c07341872ce0e5f0f8c6eff37e310aa57617a6e
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Puzzle Game"));
<<<<<<< HEAD
    setFixedSize(1000, 700);
=======
    setFixedSize(800, 600);

    // Apply background
    applyBackground(this,":/images/background.jpg");
>>>>>>> 2c07341872ce0e5f0f8c6eff37e310aa57617a6e

    // Set background image
    QWidget *central = new QWidget(this);
    central->setStyleSheet("QWidget {"
                           "background-image: url(:/images/background.png);"
                           "background-position: center;"
                           "background-repeat: no-repeat;"
                           "background-size: cover;"
                           "}");
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

    // Title - make it larger
    QLabel *titleLabel = new QLabel("PUZZLE GAME");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: white;"
                              "font-size: 42px;"
                              "font-weight: bold;"
                              "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
                              "margin-bottom: 40px;"
                              "}");

    // Buttons - make them larger
    QPushButton *defaultPuzzleBtn = createGlassButton("Start Game");
    QPushButton *generatePuzzleBtn = createGlassButton("Generate Puzzle");
    QPushButton *leaderboardBtn = createGlassButton("Leaderboard");

    // Create a vertical layout for the buttons to center them
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(20);
    buttonLayout->setAlignment(Qt::AlignCenter); // Center the buttons

    buttonLayout->addWidget(defaultPuzzleBtn, 0, Qt::AlignCenter);
    buttonLayout->addWidget(generatePuzzleBtn, 0, Qt::AlignCenter);
    buttonLayout->addWidget(leaderboardBtn, 0, Qt::AlignCenter);

    // Add stretch before and after the buttons to center them vertically
    overlayLayout->addStretch(); // Top stretch
    overlayLayout->addWidget(titleLabel);
    overlayLayout->addLayout(buttonLayout); // Add the centered button layout
    overlayLayout->addStretch(); // Bottom stretch

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
    button->setFixedSize(350, 80); // Increased from 250x60 to 350x80
    button->setStyleSheet(
        "QPushButton {"
        "background: rgba(255, 255, 255, 100);"
        "border: 2px solid rgba(255, 255, 255, 150);"
        "border-radius: 15px;"
        "color: white;"
        "font-size: 18px;" // Increased font size
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
<<<<<<< HEAD
    auto *win = new DefaultPuzzleMenuWindow(dbManager, nullptr);  // ← Change to nullptr
=======
    auto *win = new DefaultPuzzleMenuWindow(dbManager);
>>>>>>> 2c07341872ce0e5f0f8c6eff37e310aa57617a6e
    win->setAttribute(Qt::WA_DeleteOnClose);
    MainWindow::applyBackground(win, ":/images/background.jpg"); // Apply same background
    win->show();
}

void MainWindow::openGeneratePuzzleMenu()
{
<<<<<<< HEAD
    auto *win = new GeneratePuzzleMenuWindow(dbManager, nullptr); // ← Change 'this' to 'nullptr'
=======
    auto *win = new GeneratePuzzleMenuWindow(dbManager);
>>>>>>> 2c07341872ce0e5f0f8c6eff37e310aa57617a6e
    win->setAttribute(Qt::WA_DeleteOnClose);
    MainWindow::applyBackground(win); // Apply same background
    win->show();
}
