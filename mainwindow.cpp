// mainwindow.cpp
#include "mainwindow.h"
#include "defaultpuzzlemenuwindow.h"
#include "generatepuzzlemenuwindow.h"
#include "databasemanager.h"
#include "leaderboard.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QSpacerItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Puzzle Game"));
    setFixedSize(1200, 800); // Larger main window

    // Central widget
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Apply background wallpaper
    applyBackground(central, ":/images/background.png");

    // Main vertical layout
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(100, 80, 100, 80); // spacing from edges
    mainLayout->setSpacing(60);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Title
    QLabel *titleLabel = new QLabel("PUZZLE GAME");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "color: white;"
        "font-size: 60px;"
        "font-weight: bold;"
        "text-shadow: 2px 2px 6px rgba(0,0,0,0.7);"
        "}"
        );

    // Buttons
    QPushButton *defaultPuzzleBtn = createGlassButton("Start Game", 400, 100, 24);
    QPushButton *generatePuzzleBtn = createGlassButton("Generate Puzzle", 400, 100, 24);
    QPushButton *leaderboardBtn = createGlassButton("Leaderboard", 400, 100, 24);

    // Add widgets to layout
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(defaultPuzzleBtn, 0, Qt::AlignCenter);
    mainLayout->addWidget(generatePuzzleBtn, 0, Qt::AlignCenter);
    mainLayout->addWidget(leaderboardBtn, 0, Qt::AlignCenter);

    // Initialize database and leaderboard
    Database *db = new Database();
    db->initDatabase();
    dbManager = new DatabaseManager(db);

    Leaderboard *leaderboard = new Leaderboard(dbManager);
    leaderboard->setWindowTitle("排行榜");
    leaderboard->resize(600, 500);

    // Connect buttons
    connect(defaultPuzzleBtn, &QPushButton::clicked, this, &MainWindow::openDefaultPuzzleMenu);
    connect(generatePuzzleBtn, &QPushButton::clicked, this, &MainWindow::openGeneratePuzzleMenu);
    connect(leaderboardBtn, &QPushButton::clicked, [=]() {
        leaderboard->refresh(10);
        leaderboard->show();
    });
}

// Static helper to apply background to any widget
void MainWindow::applyBackground(QWidget *widget, const QString &imagePath)
{
    widget->setStyleSheet(
        QString(
            "QWidget {"
            "background-image: url(%1);"
            "background-position: center;"
            "background-repeat: no-repeat;"
            "background-size: cover;"
            "}"
            ).arg(imagePath)
        );
}

// Helper function to create buttons with custom size and font
QPushButton* MainWindow::createGlassButton(const QString &text, int width, int height, int fontSize)
{
    QPushButton *button = new QPushButton(text);
    button->setFixedSize(width, height);
    button->setStyleSheet(
        QString(
            "QPushButton {"
            "background: rgba(255, 255, 255, 120);"
            "border: 2px solid rgba(255, 255, 255, 180);"
            "border-radius: 15px;"
            "color: white;"
            "font-size: %1px;"
            "font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "background: rgba(255, 255, 255, 180);"
            "border: 2px solid rgba(255, 255, 255, 220);"
            "}"
            "QPushButton:pressed {"
            "background: rgba(255, 255, 255, 200);"
            "}"
            ).arg(fontSize)
        );
    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void MainWindow::openDefaultPuzzleMenu()
{
    auto *win = new DefaultPuzzleMenuWindow(dbManager);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}

void MainWindow::openGeneratePuzzleMenu()
{
    auto *win = new GeneratePuzzleMenuWindow(dbManager);
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}
