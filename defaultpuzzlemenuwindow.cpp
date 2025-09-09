#include "defaultpuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include "mainwindow.h" // For background function
#include "databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>

DefaultPuzzleMenuWindow::DefaultPuzzleMenuWindow(DatabaseManager *db, QWidget *parent)
    : QWidget(parent), dbManager(db)
{
    setWindowTitle("Choose Level");
    setFixedSize(1700, 1000);

    // Method 1: Use palette instead of stylesheet
    QPixmap background(":/images/background.png");
    if (!background.isNull()) {
        QPalette palette;
        palette.setBrush(this->backgroundRole(), QBrush(background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    } else {
        // Fallback to gradient
        this->setStyleSheet(
            "QWidget {"
            "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #667eea, stop: 1 #764ba2);"
            "}"
            );
    }

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Make overlay completely transparent
    QFrame *overlay = new QFrame();
    overlay->setStyleSheet("QFrame {"
                           "background: transparent;"
                           "border: none;"
                           "}");


    QVBoxLayout *overlayLayout = new QVBoxLayout(overlay);
    overlayLayout->setSpacing(25);
    overlayLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    QLabel *titleLabel = new QLabel("CHOOSE DIFFICULTY LEVEL");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: white;"
                              "font-size: 35px;"
                              "font-weight: bold;"
                              "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
                              "margin-bottom: 30px;"
                              "}");

    // Level buttons with beautiful styling
    QPushButton *btn3x3 = new QPushButton("🎮  LEVEL 1 - EASY (3x3)");
    QPushButton *btn5x5 = new QPushButton("🎯  LEVEL 2 - MEDIUM (5x5)");
    QPushButton *btn7x7 = new QPushButton("🔥  LEVEL 3 - HARD (7x7)");
    QPushButton *backBtn = new QPushButton("◀  BACK TO MENU");

    // Style for level buttons
    QString levelButtonStyle =
        "QPushButton {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6a11cb, stop: 1 #2575fc);"
        "border: 2px solid rgba(255, 255, 255, 100);"
        "border-radius: 12px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "padding: 20px;"
        "min-width: 300px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #2575fc, stop: 1 #6a11cb);"
        "border: 2px solid rgba(255, 255, 255, 200);"
        "transform: scale(1.05);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4d00a1, stop: 1 #1a5fb4);"
        "}";

    // Style for back button
    QString backButtonStyle =
        "QPushButton {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #6a11cb, stop: 1 #2575fc);"
        "border: 2px solid rgba(255, 255, 255, 100);"
        "border-radius: 12px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "padding: 20px;"
        "min-width: 300px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #2575fc, stop: 1 #6a11cb);"
        "border: 2px solid rgba(255, 255, 255, 200);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #4d00a1, stop: 1 #1a5fb4);"
        "}";

    // Apply styles
    btn3x3->setStyleSheet(levelButtonStyle);
    btn5x5->setStyleSheet(levelButtonStyle);
    btn7x7->setStyleSheet(levelButtonStyle);
    backBtn->setStyleSheet(backButtonStyle);

    // Set cursor for better UX
    btn3x3->setCursor(Qt::PointingHandCursor);
    btn5x5->setCursor(Qt::PointingHandCursor);
    btn7x7->setCursor(Qt::PointingHandCursor);
    backBtn->setCursor(Qt::PointingHandCursor);

    // Add some description labels
    QLabel *descLabel = new QLabel("Select a difficulty level to start the puzzle game:");
    descLabel->setStyleSheet("QLabel { color: white; font-size: 17px;font-weight:bold; margin-bottom: 10px; }");
    descLabel->setAlignment(Qt::AlignCenter);

    // Layout organization
    overlayLayout->addStretch();
    overlayLayout->addWidget(titleLabel);
    overlayLayout->addWidget(descLabel);
    overlayLayout->addSpacing(20);

    overlayLayout->addWidget(btn3x3, 0, Qt::AlignCenter);
    overlayLayout->addWidget(btn5x5, 0, Qt::AlignCenter);
    overlayLayout->addWidget(btn7x7, 0, Qt::AlignCenter);

    overlayLayout->addSpacing(40);
    overlayLayout->addWidget(backBtn, 0, Qt::AlignCenter);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlay);

    // Connect signals - KEEP THE ORIGINAL LOGIC
    connect(btn3x3, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle3x3);
    connect(btn5x5, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle5x5);
    connect(btn7x7, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::startPuzzle7x7);
    connect(backBtn, &QPushButton::clicked, this, &DefaultPuzzleMenuWindow::close);
}

// KEEP THE ORIGINAL PUZZLE LOGIC EXACTLY AS IT WAS
void DefaultPuzzleMenuWindow::startPuzzle3x3()
{
    MainWindowPuzzle *win = new MainWindowPuzzle(":/images/colorImg.bmp", ":/images/greyImg.bmp", 3, 300, dbManager, this);
    win->show();
}

void DefaultPuzzleMenuWindow::startPuzzle5x5()
{
    MainWindowPuzzle *win = new MainWindowPuzzle(":/images/colorImg.bmp", ":/images/greyImg.bmp", 5, 300, dbManager, this);
    win->show();
}

void DefaultPuzzleMenuWindow::startPuzzle7x7()
{
    MainWindowPuzzle *win = new MainWindowPuzzle(":/images/colorImg.bmp", ":/images/greyImg.bmp", 7, 300, dbManager, this);
    win->show();
}
