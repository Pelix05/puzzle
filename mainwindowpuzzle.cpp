#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>

MainWindowPuzzle::MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int grid, int timerSeconds, QWidget *parent)
    : QMainWindow(parent), gridSize(grid), timeLeft(timerSeconds)
{
    puzzleWidget = new PuzzleWidget(400);
    piecesList = new PiecesList(puzzleWidget->width(), this);
    greyImage = new QLabel;

    timerLabel = new QLabel(QString("Time: %1 s").arg(timeLeft));
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindowPuzzle::updateTimer);
    timer->start(1000);

    setupWidgets();
    loadImage(colorPath, greyPath);
}

void MainWindowPuzzle::setupWidgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(greyImage);
    leftLayout->addWidget(piecesList);
    leftLayout->addWidget(timerLabel);

    frameLayout->addLayout(leftLayout);
    frameLayout->addWidget(puzzleWidget);
    setCentralWidget(frame);
}

void MainWindowPuzzle::loadImage(const QString &fileName, const QString &grey_fileName)
{
    QPixmap newImage;
    if(!newImage.load(fileName)) return;
    puzzleImage = newImage;

    QPixmap greyPix;
    if(!greyPix.load(grey_fileName)) return;
    greyPix = greyPix.scaled(QSize(100,100), Qt::KeepAspectRatio);
    greyImage->setPixmap(greyPix);

    setupPuzzle();
}

void MainWindowPuzzle::setupPuzzle()
{
    int size = qMin(puzzleImage.width(), puzzleImage.height());
    puzzleImage = puzzleImage.copy((puzzleImage.width() - size)/2,
                                   (puzzleImage.height() - size)/2,
                                   size, size).scaled(puzzleWidget->width(),
                              puzzleWidget->height(),
                              Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);

    piecesList->clear();

    int pieceSize = puzzleWidget->width() / gridSize;

    for(int y = 0; y < gridSize; ++y)
        for(int x = 0; x < gridSize; ++x)
        {
            QPixmap pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);
            piecesList->addPiece(pieceImage, QPoint(x, y));
        }

    puzzleWidget->clear();
}

void MainWindowPuzzle::setCompleted()
{
    timer->stop();
    QMessageBox::information(this, "Puzzle Selesai", "Selamat! Puzzle selesai.");
}

void MainWindowPuzzle::updateTimer()
{
    timeLeft--;
    timerLabel->setText(QString("Time: %1 s").arg(timeLeft));
    if(timeLeft <= 0)
    {
        timer->stop();
        QMessageBox::warning(this, "Time's up!", "Waktu habis!");
    }
}

