#include "mainwindow.h"
#include "defaultpuzzlemenuwindow.h"
#include "generatepuzzlemenuwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>
#include <QFont>
#include <QStyle>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Puzzle Game"));
    setFixedSize(800, 600);

    // Set background image
    QWidget *central = new QWidget(this);
    central->setStyleSheet("QWidget {"
                           "background-image: url(:/images/background.jpg);"
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

    // Title
    QLabel *titleLabel = new QLabel("拼图游戏");
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
    QPushButton *defaultPuzzleBtn = createGlassButton("Start Puzzle ");
    QPushButton *generatePuzzleBtn = createGlassButton("Generate Puzzle");

    overlayLayout->addWidget(defaultPuzzleBtn);
    overlayLayout->addWidget(generatePuzzleBtn);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlay);

    connect(defaultPuzzleBtn, &QPushButton::clicked, this, &MainWindow::openDefaultPuzzleMenu);
    connect(generatePuzzleBtn, &QPushButton::clicked, this, &MainWindow::openGeneratePuzzleMenu);
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
    auto *win = new DefaultPuzzleMenuWindow;
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}

void MainWindow::openGeneratePuzzleMenu()
{
    auto *win = new GeneratePuzzleMenuWindow;
    win->setAttribute(Qt::WA_DeleteOnClose);
    win->show();
}
