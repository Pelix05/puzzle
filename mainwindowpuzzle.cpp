#include "mainwindowpuzzle.h"
#include "puzzle/databasemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QPushButton>
#include <QRandomGenerator>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QInputDialog>
#include <QMessageBox>



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
    // Set solid background color for the whole window
    this->setStyleSheet("QMainWindow { background-color: #2c3e50; }"); // dark color

    // Central frame for everything
    QFrame *centralFrame = new QFrame(this);
    centralFrame->setStyleSheet("QFrame { background: transparent; }");
    setCentralWidget(centralFrame);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralFrame);

    // --- Left panel (pieces, timer, buttons) ---
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // Semi-transparent box for puzzle pieces list
    QFrame *piecesBox = new QFrame;
    piecesBox->setStyleSheet(
        "QFrame {"
        "background-color: rgba(255, 255, 255, 50);" // semi-transparent white
        "border-radius: 15px;"
        "padding: 10px;"
        "}"
        );
    QVBoxLayout *piecesLayout = new QVBoxLayout(piecesBox);
    piecesLayout->addWidget(greyImage);
    piecesLayout->addWidget(piecesList);

    // Add timer and control buttons
    QPushButton *backBtn = new QPushButton("Back");
    QPushButton *resetButton = new QPushButton("Reset");
    QPushButton *undoButton = new QPushButton("Undo");
    QPushButton *saveBtn = new QPushButton("Save");
    QPushButton *loadBtn = new QPushButton("Load");
    leftLayout->addWidget(saveBtn);
    leftLayout->addWidget(loadBtn);

    connect(saveBtn, &QPushButton::clicked, this, &MainWindowPuzzle::saveProgress);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindowPuzzle::loadProgress);

    leftLayout->addWidget(backBtn);
    leftLayout->addWidget(piecesBox); // semi-transparent box for pieces list
    leftLayout->addWidget(timerLabel);
    leftLayout->addWidget(resetButton);
    leftLayout->addWidget(undoButton);

    mainLayout->addLayout(leftLayout);

    // --- Right panel: Puzzle board ---
    // Semi-transparent box for puzzle board
    QFrame *puzzleBox = new QFrame;
    puzzleBox->setStyleSheet(
        "QFrame {"
        "background-color: rgba(255, 255, 255, 50);" // semi-transparent white
        "border-radius: 15px;"
        "}"
        );
    QVBoxLayout *puzzleLayout = new QVBoxLayout(puzzleBox);
    puzzleLayout->addWidget(puzzleWidget);

    mainLayout->addWidget(puzzleBox); // place puzzle board inside box

    // Connect buttons
    connect(backBtn, &QPushButton::clicked, this, &MainWindowPuzzle::close);
    connect(resetButton, &QPushButton::clicked, this, &MainWindowPuzzle::resetPuzzle);
    connect(undoButton, &QPushButton::clicked, this, &MainWindowPuzzle::undoMove);
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


    bool ok;
    QString username = QInputDialog::getText(this, "Enter your name",
                                             "Player Name:", QLineEdit::Normal,
                                             "", &ok);
    if (ok && !username.isEmpty()) {
        int playerId = dbManager->insertPlayer(username);
        if (playerId == -1) {
            // 用户名已存在，查询ID
            QSqlQuery query;
            query.prepare("SELECT player_id FROM players WHERE name = :name");
            query.bindValue(":name", username);
            query.exec();
            if (query.next())
                playerId = query.value(0).toInt();
        }


        int pid = playerId;
        int dur = duration;
        int st = steps;
        int lev = level;

        dbManager->insertRecord(pid, dur, st, lev);
    }
}

void MainWindowPuzzle::saveProgress() {
    QJsonObject saveData;
    saveData["gridSize"] = gridSize;
    saveData["timeLeft"] = timeLeft;

    // ✅ Pieces di puzzleWidget
    QJsonArray placedPieces;
    for (const auto &p : puzzleWidget->getPieces()) {
        QJsonObject obj;
        obj["x"] = p.location.x();
        obj["y"] = p.location.y();
        obj["rotation"] = p.rotation;
        obj["rectX"] = p.rect.x();
        obj["rectY"] = p.rect.y();
        placedPieces.append(obj);
    }
    saveData["placedPieces"] = placedPieces;

    // ✅ Pieces di piecesList
    QJsonArray remaining;
    for (int i=0; i<piecesList->count(); i++) {
        QListWidgetItem *item = piecesList->item(i);
        QPoint loc = item->data(Qt::UserRole+1).toPoint();
        int rot = item->data(Qt::UserRole+2).toInt();
        QJsonObject obj;
        obj["x"] = loc.x();
        obj["y"] = loc.y();
        obj["rotation"] = rot;
        remaining.append(obj);
    }
    saveData["remainingPieces"] = remaining;

    // Tulis ke file
    QDir().mkpath("saves");
    QFile file("saves/save_" + QString::number(QDateTime::currentSecsSinceEpoch()) + ".json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(saveData).toJson());
        file.close();
        QMessageBox::information(this, "Saved", "Progress saved!");
    }
}

void MainWindowPuzzle::loadProgress() {
    QString dir = "saves";
    QDir saveDir(dir);
    QStringList files = saveDir.entryList(QStringList() << "*.json", QDir::Files);

    if (files.isEmpty()) {
        QMessageBox::warning(this, "Load", "No saved progress found.");
        return;
    }

    bool ok;
    QString fileName = QInputDialog::getItem(this, "Load Progress",
                                             "Choose a save:", files, 0, false, &ok);
    if (!ok || fileName.isEmpty()) return;

    QFile file(dir + "/" + fileName);
    if (!file.open(QIODevice::ReadOnly)) return;
    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject saveData = doc.object();
    gridSize = saveData["gridSize"].toInt();
    timeLeft = saveData["timeLeft"].toInt();

    puzzleWidget->clear();
    piecesList->clear();

    // Load placed pieces
    QJsonArray placed = saveData["placedPieces"].toArray();
    for (auto v : placed) {
        QJsonObject obj = v.toObject();
        QPoint loc(obj["x"].toInt(), obj["y"].toInt());
        int rot = obj["rotation"].toInt();
        QRect rect(obj["rectX"].toInt(), obj["rectY"].toInt(), puzzleWidget->pieceSize(), puzzleWidget->pieceSize());

        QPixmap pieceImg = puzzleImage.copy(loc.x()*puzzleWidget->pieceSize(), loc.y()*puzzleWidget->pieceSize(),
                                            puzzleWidget->pieceSize(), puzzleWidget->pieceSize());

        puzzleWidget->addPieceWithRotation(pieceImg, loc, rot);
    }

    // Load remaining pieces
    QJsonArray remaining = saveData["remainingPieces"].toArray();
    for (auto v : remaining) {
        QJsonObject obj = v.toObject();
        QPoint loc(obj["x"].toInt(), obj["y"].toInt());
        int rot = obj["rotation"].toInt();

        QPixmap pieceImg = puzzleImage.copy(loc.x()*puzzleWidget->pieceSize(), loc.y()*puzzleWidget->pieceSize(),
                                            puzzleWidget->pieceSize(), puzzleWidget->pieceSize());
        piecesList->addPiece(pieceImg, loc, rot);
    }

    timerLabel->setText(QString("Time: %1 s").arg(timeLeft));
    timer->start(1000);

    QMessageBox::information(this, "Loaded", "Progress loaded!");
}
