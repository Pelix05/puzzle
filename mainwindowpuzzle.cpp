#include "mainwindowpuzzle.h"
#include "puzzle/databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QInputDialog>


MainWindowPuzzle::MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int grid, int timerSeconds, DatabaseManager *db,  QWidget *parent)
    : QMainWindow(parent), timeLeft(timerSeconds), gridSize(grid), dbManager(db), moveCount(0)
{
    // ✅ 根据 gridSize 判定难度
    if (grid == 3)
        level = 1; // 简单
    else if (grid == 5)
        level = 2; // 中等
    else if (grid == 7)
        level = 3; // 困难
    else
        level = 0; // 默认

    puzzleWidget = new PuzzleWidget(400, gridSize, this);
    piecesList = new PiecesList(puzzleWidget->width(), this);
    greyImage = new QLabel;

    timerLabel = new QLabel(QString("Time: %1 s").arg(timeLeft));
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, &MainWindowPuzzle::updateTimer);
    connect(puzzleWidget, &PuzzleWidget::puzzleCompleted,
            this, &MainWindowPuzzle::setCompleted);
    connect(puzzleWidget, &PuzzleWidget::piecePlaced, this,
            [this](QPixmap pix, QPoint loc, QRect rect){
                moveHistory.append({pix, loc, rect});
                moveCount ++;
            });


    timer->start(1000);

    setupWidgets();
    loadImage(colorPath, greyPath);
}

void MainWindowPuzzle::setupWidgets()
{
    QFrame *frame = new QFrame;
    QHBoxLayout *frameLayout = new QHBoxLayout(frame);

    QVBoxLayout *leftLayout = new QVBoxLayout;

    QPushButton *backBtn = new QPushButton("Back");
    connect(backBtn, &QPushButton::clicked, this, &MainWindowPuzzle::close);
    leftLayout->addWidget(backBtn);

    leftLayout->addWidget(greyImage);
    leftLayout->addWidget(piecesList);
    leftLayout->addWidget(timerLabel);

    QPushButton *resetButton = new QPushButton("Reset");
    QPushButton *undoButton = new QPushButton("Undo");
    connect(resetButton, &QPushButton::clicked, this, &MainWindowPuzzle::resetPuzzle);
    connect(undoButton, &QPushButton::clicked, this, &MainWindowPuzzle::undoMove);
    leftLayout->addWidget(resetButton);
    leftLayout->addWidget(undoButton);

    frameLayout->addLayout(leftLayout);
    frameLayout->addWidget(puzzleWidget);
    setCentralWidget(frame);
}

void MainWindowPuzzle::loadImage(const QString &fileName, const QString &grey_fileName)
{
    QPixmap newImage;
    if(!newImage.load(fileName)) return;
    puzzleImage = newImage;

    QImage greyImg = newImage.toImage().convertToFormat(QImage::Format_Grayscale8);
    QPixmap greyPix = QPixmap::fromImage(greyImg);

    greyPix = greyPix.scaled(QSize(100,100), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    greyImage->setPixmap(greyPix);

    setupPuzzle();
}

void MainWindowPuzzle::resetPuzzle()
{
    puzzleWidget->clear();

    moveHistory.clear();

    setupPuzzle();
}

void MainWindowPuzzle::undoMove()
{
    if (moveHistory.isEmpty())
        return;

    Move last = moveHistory.takeLast();

    // Hapus dari puzzleWidget
    puzzleWidget->removePiece(last.rect);

    // Balik ke piecesList
    piecesList->addPiece(last.pixmap, last.location);

    puzzleWidget->update();
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

    int rotations[4] = {0, 90, 180, 270};

    for(int y = 0; y < gridSize; ++y)
        for(int x = 0; x < gridSize; ++x)
        {
            QPixmap pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);
            int rot = rotations[QRandomGenerator::global()->bounded(4)];
            piecesList->addPiece(pieceImage, QPoint(x, y), rot);
        }
}


void MainWindowPuzzle::setCompleted()
{
    timer->stop();
    QMessageBox::information(this, "Puzzle Selesai", "Selamat! Puzzle selesai.");
    promptAndSaveRecord();
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

void MainWindowPuzzle::promptAndSaveRecord()
{
    int duration = 1000 - timeLeft;
    int steps = moveCount;

     qDebug() << "[Game] Calculated duration:" << duration << "steps:" << steps;


    bool ok;
    QString username = QInputDialog::getText(this, "Enter your name",
                                             "Player Name:", QLineEdit::Normal,
                                             "", &ok);

    qDebug() << "[Game] User input:" << username << "ok?" << ok;

    if (ok && !username.isEmpty()) {
        int playerId = dbManager->insertPlayer(username);
        qDebug() << "[Database] insertPlayer returned playerId:" << playerId;

        if (playerId == -1) {
            // 用户名已存在，查询ID
            QSqlQuery query;
            query.prepare("SELECT player_id FROM players WHERE name = :name");
            query.bindValue(":name", username);
            query.exec();
            if (!query.exec()) {
                qDebug() << "[Database] Failed to query existing player:" << query.lastError().text();
            } else if (query.next()) {
                playerId = query.value(0).toInt();
                qDebug() << "[Database] Existing playerId found:" << playerId;
            } else {
                qDebug() << "[Database] No player found with name:" << username;
            }
        }


        int pid = playerId;
        int dur = duration;
        int st = steps;
        int lev = level;

        qDebug() << "[Game] Ready to insert record -> playerId:" << pid
                 << "duration:" << dur << "steps:" << st << "level:" << lev;

        dbManager->insertRecord(pid, dur, st, lev);

        qDebug() << "[Game] Record insertion called for playerId:" << pid;
    }else {
        qDebug() << "[Game] User cancelled or entered empty name";
    }
}

