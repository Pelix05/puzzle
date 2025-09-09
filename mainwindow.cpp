// mainwindow.cpp
#include "mainwindow.h"
#include "defaultpuzzlemenuwindow.h"
#include "generatepuzzlemenuwindow.h"
#include "databasemanager.h"
#include "leaderboard.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QFont>
#include <QStyle>

// Static function to apply background to any QWidget
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

// Constructor
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Puzzle Game"));
    setFixedSize(800, 600);

    // Central widget
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    applyBackground(central, ":/images/background.jpg");

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Semi-transparent overlay
    QFrame *overlay = new QFrame();
    overlay->setStyleSheet(
        "QFrame {"
        "background-image: url(:/images/background.png);"
        "background-position: center;"
        "background-repeat: no-repeat;"
        "background-size: cover;"
        "background-color: rgba(0,0,0,150);"
        "border: none;"
        "}"
        );


    QVBoxLayout *overlayLayout = new QVBoxLayout(overlay);
    overlayLayout->setSpacing(30);
    overlayLayout->setContentsMargins(50, 50, 50, 50);

    // Title
    QLabel *titleLabel = new QLabel("PUZZLE GAME");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "QLabel {"
        "color: white;"
        "font-size: 42px;"
        "font-weight: bold;"
        "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
        "margin-bottom: 40px;"
        "}"
        );

    // Buttons
    QPushButton *defaultPuzzleBtn = createGlassButton("Start Game");
    QPushButton *generatePuzzleBtn = createGlassButton("Generate Puzzle");
    QPushButton *leaderboardBtn = createGlassButton("Leaderboard");

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setSpacing(20);
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(defaultPuzzleBtn, 0, Qt::AlignCenter);
    buttonLayout->addWidget(generatePuzzleBtn, 0, Qt::AlignCenter);
    buttonLayout->addWidget(leaderboardBtn, 0, Qt::AlignCenter);

    overlayLayout->addStretch();
    overlayLayout->addWidget(titleLabel);
    overlayLayout->addLayout(buttonLayout);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlay);

    // Connect buttons
    connect(defaultPuzzleBtn, &QPushButton::clicked, this, &MainWindow::openDefaultPuzzleMenu);
    connect(generatePuzzleBtn, &QPushButton::clicked, this, &MainWindow::openGeneratePuzzleMenu);

    // Initialize database and leaderboard
    Database *db = new Database();
    db->initDatabase();
    dbManager = new DatabaseManager(db);

    Leaderboard *leaderboard = new Leaderboard(dbManager);
    leaderboard->setWindowTitle("排行榜");
    leaderboard->resize(500, 400);

    connect(leaderboardBtn, &QPushButton::clicked, [=]() {
        leaderboard->refresh(10); // refresh Top 10
        leaderboard->show();      // show leaderboard
    });
}

// Create a glass-style QPushButton
QPushButton* MainWindow::createGlassButton(const QString &text)
{
    QPushButton *button = new QPushButton(text);
    button->setFixedSize(350, 80);
    button->setStyleSheet(
        "QPushButton {"
        "background: rgba(255, 255, 255, 100);"
        "border: 2px solid rgba(255, 255, 255, 150);"
        "border-radius: 15px;"
        "color: white;"
        "font-size: 18px;"
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
    applyBackground(win); // static background function
    win->show();
}

void MainWindow::openGeneratePuzzleMenu()
{
    auto *win = new GeneratePuzzleMenuWindow(dbManager);
    win->setAttribute(Qt::WA_DeleteOnClose);
    applyBackground(win); // static background function
    win->show();
}

