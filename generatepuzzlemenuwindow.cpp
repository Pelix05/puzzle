#include "generatepuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>
#include <QPixmap>


GeneratePuzzleMenuWindow::GeneratePuzzleMenuWindow(QWidget *parent)
    : QWidget(parent), gridSize(5), timerSeconds(300)
{
    setWindowTitle("Generate Puzzle");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *chooseBtn = new QPushButton("Select your photo");
    QLabel *gridLabel = new QLabel("Select Grid number:");
    QSpinBox *gridSpin = new QSpinBox;
    gridSpin->setRange(2, 10);
    gridSpin->setValue(gridSize);

    QLabel *timerLabel = new QLabel("Set timer (second):");
    QSpinBox *timerSpin = new QSpinBox;
    timerSpin->setRange(10, 3600);
    timerSpin->setValue(timerSeconds);

    QPushButton *startBtn = new QPushButton("Start");
    QPushButton *backBtn = new QPushButton("Back");
    layout->addWidget(backBtn);

    layout->addWidget(chooseBtn);
    layout->addWidget(gridLabel);
    layout->addWidget(gridSpin);
    layout->addWidget(timerLabel);
    layout->addWidget(timerSpin);
    layout->addWidget(startBtn);
    imagePreview = new QLabel("Puzzle priview");
    imagePreview->setAlignment(Qt::AlignCenter);
    imagePreview->setFixedSize(300, 300); // ukuran preview
    imagePreview->setStyleSheet("border: 1px solid gray;");
    layout->insertWidget(1, imagePreview);

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
        imagePreview->setPixmap(pix.scaled(imagePreview->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

void GeneratePuzzleMenuWindow::startCustomPuzzle()
{
    if(selectedImagePath.isEmpty()) return;
    MainWindowPuzzle *win = new MainWindowPuzzle(selectedImagePath, selectedImagePath, gridSize, timerSeconds);
    win->show();
}
