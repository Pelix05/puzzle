#include "generatepuzzlemenuwindow.h"
#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QLabel>
#include <QFileDialog>

GeneratePuzzleMenuWindow::GeneratePuzzleMenuWindow(QWidget *parent)
    : QWidget(parent), gridSize(5), timerSeconds(300)
{
    setWindowTitle("Generate Puzzle");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QPushButton *chooseBtn = new QPushButton("Pilih Foto");
    QLabel *gridLabel = new QLabel("Pilih ukuran grid:");
    QSpinBox *gridSpin = new QSpinBox;
    gridSpin->setRange(2, 10);
    gridSpin->setValue(gridSize);

    QLabel *timerLabel = new QLabel("Set timer (detik):");
    QSpinBox *timerSpin = new QSpinBox;
    timerSpin->setRange(10, 3600);
    timerSpin->setValue(timerSeconds);

    QPushButton *startBtn = new QPushButton("Mulai Puzzle");

    layout->addWidget(chooseBtn);
    layout->addWidget(gridLabel);
    layout->addWidget(gridSpin);
    layout->addWidget(timerLabel);
    layout->addWidget(timerSpin);
    layout->addWidget(startBtn);

    connect(chooseBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::chooseImage);
    connect(startBtn, &QPushButton::clicked, this, &GeneratePuzzleMenuWindow::startCustomPuzzle);
    connect(gridSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v){ gridSize = v; });
    connect(timerSpin, QOverload<int>::of(&QSpinBox::valueChanged), [this](int v){ timerSeconds = v; });
}

void GeneratePuzzleMenuWindow::chooseImage()
{
    selectedImagePath = QFileDialog::getOpenFileName(this, "Pilih Gambar", "", "Images (*.bmp *.jpg *.png)");
}

void GeneratePuzzleMenuWindow::startCustomPuzzle()
{
    if(selectedImagePath.isEmpty()) return;
    MainWindowPuzzle *win = new MainWindowPuzzle(selectedImagePath, selectedImagePath, gridSize, timerSeconds);
    win->show();
}
