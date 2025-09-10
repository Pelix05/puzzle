#include "generatepuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include "mainwindow.h"
#include "databasemanager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>
#include <QFrame>

GeneratePuzzleMenuWindow::GeneratePuzzleMenuWindow(DatabaseManager* db)
    : dbManager(db), gridSize(5), timerSeconds(300)
{
    setWindowTitle("Generate Puzzle");
    setFixedSize(1700, 1000);

    // Apply background using palette (like DefaultPuzzleMenuWindow)
    QPixmap background(":/images/background.png");
    if (!background.isNull()) {
        QPalette palette;
        palette.setBrush(this->backgroundRole(),
                         QBrush(background.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
        this->setAutoFillBackground(true);
    }

    // Transparent overlay for all widgets
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    QFrame *overlay = new QFrame();
    overlay->setStyleSheet("QFrame { background: transparent; border: none; }");
    QVBoxLayout *overlayLayout = new QVBoxLayout(overlay);
    overlayLayout->setSpacing(25);
    overlayLayout->setContentsMargins(40, 40, 40, 40);

    // Title
    QLabel *titleLabel = new QLabel("CREATE CUSTOM PUZZLE");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel {"
                              "color: white;"
                              "font-size: 35px;"
                              "font-weight: bold;"
                              "text-shadow: 2px 2px 4px rgba(0,0,0,0.5);"
                              "margin-bottom: 20px;"
                              "}");

    // Image preview
    imagePreview = new QLabel("Select an image to preview");
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setFixedSize(400, 400);
    imagePreview->setStyleSheet(
        "QLabel {"
        "background: rgba(255,255,255,50);"
        "border: 2px dashed rgba(255,255,255,100);"
        "border-radius: 10px;"
        "color: rgba(255,255,255,150);"
        "font-size: 14px;"
        "}");

    QPushButton *chooseBtn = new QPushButton("📷 Select your photo");
    chooseBtn->setFixedSize(250, 50);

    // Style for buttons (same as DefaultPuzzleMenuWindow)
    QString buttonStyle =
        "QPushButton {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "stop:0 #6a11cb, stop:1 #2575fc);"
        "border: 2px solid rgba(255,255,255,100);"
        "border-radius: 12px;"
        "color: white;"
        "font-size: 16px;"
        "font-weight: bold;"
        "padding: 12px;"
        "min-width: 200px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "stop:0 #2575fc, stop:1 #6a11cb);"
        "border: 2px solid rgba(255,255,255,200);"
        "}"
        "QPushButton:pressed {"
        "background: qlineargradient(x1:0, y1:0, x2:0, y2:1,"
        "stop:0 #4d00a1, stop:1 #1a5fb4);"
        "}";

    chooseBtn->setStyleSheet(buttonStyle);

    // Grid & timer controls in the center
    QLabel *gridLabel = new QLabel("Select Grid number:");
    gridLabel->setStyleSheet("QLabel { color:white; font-weight:bold; font-size:16px; }");
    QSpinBox *gridSpin = new QSpinBox;
    gridSpin->setRange(2,10);
    gridSpin->setValue(gridSize);
    gridSpin->setStyleSheet("QSpinBox { background:white; border-radius:6px; padding:5px; min-width:80px; }");

    QLabel *timerLabel = new QLabel("Set timer (seconds):");
    timerLabel->setStyleSheet("QLabel { color:white; font-weight:bold; font-size:16px; }");
    QSpinBox *timerSpin = new QSpinBox;
    timerSpin->setRange(10,3600);
    timerSpin->setValue(timerSeconds);
    timerSpin->setStyleSheet("QSpinBox { background:white; border-radius:6px; padding:5px; min-width:80px; }");

    QVBoxLayout *settingsLayout = new QVBoxLayout();
    settingsLayout->setSpacing(15);
    settingsLayout->setAlignment(Qt::AlignCenter);

    QHBoxLayout *gridLayout = new QHBoxLayout();
    gridLayout->addWidget(gridLabel);
    gridLayout->addWidget(gridSpin);

    QHBoxLayout *timerLayout = new QHBoxLayout();
    timerLayout->addWidget(timerLabel);
    timerLayout->addWidget(timerSpin);

    settingsLayout->addLayout(gridLayout);
    settingsLayout->addLayout(timerLayout);

    // Start button
    QPushButton *startBtn = new QPushButton("🚀 Start Puzzle");
    startBtn->setFixedSize(250, 50);
    startBtn->setStyleSheet(buttonStyle);

    QPushButton *backBtn = new QPushButton("◀ Back to Menu");
    backBtn->setStyleSheet(buttonStyle);

    // Layout organization
    overlayLayout->addStretch();
    overlayLayout->addWidget(titleLabel);
    overlayLayout->addWidget(imagePreview, 0, Qt::AlignCenter);
    overlayLayout->addWidget(chooseBtn, 0, Qt::AlignCenter);
    overlayLayout->addSpacing(20);
    overlayLayout->addLayout(settingsLayout); // ✅ Grid + Timer in the middle
    overlayLayout->addSpacing(30);
    overlayLayout->addWidget(startBtn, 0, Qt::AlignCenter);
    overlayLayout->addSpacing(20);
    overlayLayout->addWidget(backBtn, 0, Qt::AlignCenter);
    overlayLayout->addStretch();

    mainLayout->addWidget(overlay);

    // Connections
    connect(backBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::close);
    connect(chooseBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::chooseImage);
    connect(startBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::startCustomPuzzle);
    connect(gridSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v){ gridSize = v; });
    connect(timerSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v){ timerSeconds = v; });
}

void GeneratePuzzleMenuWindow::chooseImage()
{
    selectedImagePath = QFileDialog::getOpenFileName(this, "Choose Image", "", "Images (*.bmp *.jpg *.png)");
    if (!selectedImagePath.isEmpty()) {
        QPixmap pix(selectedImagePath);
        if (!pix.isNull()) {
            imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            imagePreview->setStyleSheet("border: 2px solid rgba(255,255,255,200); border-radius: 10px;");
        }
    }
}

void GeneratePuzzleMenuWindow::startCustomPuzzle()
{
    if(selectedImagePath.isEmpty()) {
        imagePreview->setText("Please select an image first!");
        return;
    }

    MainWindowPuzzle *win = new MainWindowPuzzle(
        selectedImagePath, selectedImagePath,
        gridSize, timerSeconds, dbManager, "custom", nullptr
        );
    win->setAttribute(Qt::WA_DeleteOnClose);

    win->show();
    this->close(); // close the generate puzzle menu
}

