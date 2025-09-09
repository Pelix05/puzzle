#include "generatepuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include "mainwindow.h" // Add this include for the background function
#include "databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QFrame>
#include <QGroupBox>


GeneratePuzzleMenuWindow::GeneratePuzzleMenuWindow(DatabaseManager* db, QWidget *parent)
    : QWidget(parent), dbManager(db) , gridSize(5), timerSeconds(300)
{
    setWindowTitle("Generate Puzzle");
    setFixedSize(800, 600); // Set fixed size for consistency

    // Apply the same background as main window
    MainWindow::applyBackground(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Semi-transparent overlay
    QFrame *overlay = new QFrame();
    overlay->setStyleSheet("QFrame {"
                           "background: rgba(0, 0, 0, 150);"
                           "border: none;"
                           "border-radius: 10px;"
                           "}");

    QVBoxLayout *overlayLayout = new QVBoxLayout(overlay);
    overlayLayout->setSpacing(15);
    overlayLayout->setContentsMargins(30, 30, 30, 30);

    // Title
    QLabel *titleLabel = new QLabel("CREATE CUSTOM PUZZLE");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: white;"
                              "font-size: 24px;"
                              "font-weight: bold;"
                              "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
                              "margin-bottom: 20px;"
                              "}");

    // Back button at top
    QPushButton *backBtn = new QPushButton("◀ Back");
    backBtn->setStyleSheet(
        "QPushButton {"
        "background: rgba(255, 255, 255, 100);"
        "border: 1px solid rgba(255, 255, 255, 150);"
        "border-radius: 5px;"
        "color: white;"
        "font-size: 12px;"
        "padding: 5px 10px;"
        "}"
        "QPushButton:hover {"
        "background: rgba(255, 255, 255, 150);"
        "}"
        );

    // Image preview
    imagePreview = new QLabel("Select an image to preview");
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setFixedSize(300, 300);
    imagePreview->setStyleSheet("QLabel {"
                                "background: rgba(255, 255, 255, 50);"
                                "border: 2px dashed rgba(255, 255, 255, 100);"
                                "border-radius: 10px;"
                                "color: rgba(255, 255, 255, 150);"
                                "font-size: 14px;"
                                "}");

    // Control buttons and inputs
    QPushButton *chooseBtn = new QPushButton("📷 Select your photo");

    QLabel *gridLabel = new QLabel("Select Grid number:");
    QSpinBox *gridSpin = new QSpinBox;
    gridSpin->setRange(2, 10);
    gridSpin->setValue(gridSize);

    QLabel *timerLabel = new QLabel("Set timer (seconds):");
    QSpinBox *timerSpin = new QSpinBox;
    timerSpin->setRange(10, 3600);
    timerSpin->setValue(timerSeconds);

    QPushButton *startBtn = new QPushButton("🚀 Start Puzzle");

    // Style definitions
    QString labelStyle = "QLabel { color: white; font-weight: bold; font-size: 14px; }";
    QString spinBoxStyle =
        "QSpinBox {"
        "background: white;"
        "border: 1px solid #ccc;"
        "border-radius: 4px;"
        "padding: 5px;"
        "font-size: 14px;"
        "min-width: 60px;"
        "}";

    QString buttonStyle =
        "QPushButton {"
        "background: rgba(255, 255, 255, 100);"
        "border: 2px solid rgba(255, 255, 255, 150);"
        "border-radius: 8px;"
        "color: white;"
        "font-size: 14px;"
        "font-weight: bold;"
        "padding: 10px;"
        "min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "background: rgba(255, 255, 255, 150);"
        "}"
        "QPushButton:pressed {"
        "background: rgba(255, 255, 255, 200);"
        "}";

    // Apply styles
    gridLabel->setStyleSheet(labelStyle);
    timerLabel->setStyleSheet(labelStyle);
    gridSpin->setStyleSheet(spinBoxStyle);
    timerSpin->setStyleSheet(spinBoxStyle);
    chooseBtn->setStyleSheet(buttonStyle);
    startBtn->setStyleSheet(buttonStyle);

    // Set cursor for interactive elements
    chooseBtn->setCursor(Qt::PointingHandCursor);
    startBtn->setCursor(Qt::PointingHandCursor);
    backBtn->setCursor(Qt::PointingHandCursor);
    gridSpin->setCursor(Qt::PointingHandCursor);
    timerSpin->setCursor(Qt::PointingHandCursor);

    // Layout organization
    overlayLayout->addWidget(backBtn, 0, Qt::AlignLeft);
    overlayLayout->addWidget(titleLabel);

    // Image preview section
    overlayLayout->addWidget(imagePreview, 0, Qt::AlignCenter);
    overlayLayout->addWidget(chooseBtn, 0, Qt::AlignCenter);

    // Settings section
    QFrame *settingsFrame = new QFrame();
    settingsFrame->setStyleSheet("QFrame { background: rgba(255, 255, 255, 30); border-radius: 8px; padding: 15px; }");
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsFrame);
    settingsLayout->setSpacing(10);

    // Grid setting
    QHBoxLayout *gridLayout = new QHBoxLayout();
    gridLayout->addWidget(gridLabel);
    gridLayout->addWidget(gridSpin);
    gridLayout->addStretch();

    // Timer setting
    QHBoxLayout *timerLayout = new QHBoxLayout();
    timerLayout->addWidget(timerLabel);
    timerLayout->addWidget(timerSpin);
    timerLayout->addStretch();

    settingsLayout->addLayout(gridLayout);
    settingsLayout->addLayout(timerLayout);

    overlayLayout->addWidget(settingsFrame);
    overlayLayout->addWidget(startBtn, 0, Qt::AlignCenter);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlay);

    // Connections
    connect(chooseBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::chooseImage);
    connect(startBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::startCustomPuzzle);
    connect(gridSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v){ gridSize = v; });
    connect(timerSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v){ timerSeconds = v; });
    connect(backBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::close);
}

void GeneratePuzzleMenuWindow::chooseImage()
{
    selectedImagePath = QFileDialog::getOpenFileName(this, "Choose Image", "", "Images (*.bmp *.jpg *.png)");
    if (!selectedImagePath.isEmpty()) {
        QPixmap pix(selectedImagePath);
        if (!pix.isNull()) {
            imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imagePreview->setStyleSheet("border: 2px solid rgba(255, 255, 255, 200); border-radius: 10px;");
        }
    }
}

void GeneratePuzzleMenuWindow::startCustomPuzzle()
{
    if(selectedImagePath.isEmpty()) {
        // Show warning if no image selected
        imagePreview->setStyleSheet("QLabel {"
                                    "background: rgba(255, 255, 255, 50);"
                                    "border: 2px dashed rgba(255, 100, 100, 200);"
                                    "border-radius: 10px;"
                                    "color: rgba(255, 100, 100, 200);"
                                    "font-size: 14px;"
                                    "}");
        imagePreview->setText("Please select an image first!");
        return;
    }

    MainWindowPuzzle *win = new MainWindowPuzzle(selectedImagePath, selectedImagePath, gridSize, timerSeconds, dbManager, this);
    win->setAttribute(Qt::WA_DeleteOnClose);
    MainWindow::applyBackground(win); // Apply same background to puzzle window
    win->show();
    this->close(); // Close the menu window
}
