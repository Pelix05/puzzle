#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
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

#include <QResizeEvent>


void PuzzleBoardBox::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(255, 255, 255, 150));
    pen.setWidth(2);
    painter.setPen(pen);

    int boardSize = qMin(width(), height());
    int pieceSize = boardSize / m_gridSize;

    // Draw internal grid lines
    for (int i = 1; i < m_gridSize; ++i)
        painter.drawLine(i * pieceSize, 0, i * pieceSize, boardSize);

    for (int i = 1; i < m_gridSize; ++i)
        painter.drawLine(0, i * pieceSize, boardSize, i * pieceSize);

    // Draw outer border (right & bottom)
    painter.drawRect(0, 0, pieceSize * m_gridSize, pieceSize * m_gridSize);
}



MainWindowPuzzle::MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int grid, int timerSeconds, DatabaseManager *db,const QString &mode, QWidget *parent)
    : QMainWindow(parent), timeLeft(timerSeconds), gridSize(grid), dbManager(db), moveCount(0),gameMode(mode)
{
    // Set window size
    setMinimumSize(1700, 1000);         // force a large window
    setWindowState(Qt::WindowMaximized); // open maximized (optional)

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
    timerLabel->setAlignment(Qt::AlignCenter);
    timerLabel->setStyleSheet(
        "QLabel {"
        "background-color: rgba(0,0,0,180);"
        "color: white;"
        "font-weight: bold;"
        "font-size: 20px;"
        "padding: 10px;"
        "border-radius: 10px;"
        "min-width: 120px;"
        "}"
        );

    timer = new QTimer(this);
    stepLabel = new QLabel(QString("Steps: %1").arg(moveCount));
    stepLabel->setAlignment(Qt::AlignCenter);
    stepLabel->setStyleSheet(
        "QLabel {"
        "background-color: rgba(0,0,0,180);"
        "color: white;"
        "font-weight: bold;"
        "font-size: 20px;"
        "padding: 10px;"
        "border-radius: 10px;"
        "min-width: 120px;"
        "}"
        );


    connect(puzzleWidget, &PuzzleWidget::pieceRotated, this, &MainWindowPuzzle::incrementStepCount);
    connect(timer, &QTimer::timeout, this, &MainWindowPuzzle::updateTimer);
    connect(puzzleWidget, &PuzzleWidget::puzzleCompleted, this, &MainWindowPuzzle::setCompleted);
    connect(puzzleWidget, &PuzzleWidget::piecePlaced, this, [this](QPixmap pix, QPoint loc, QRect rect){
        moveHistory.append({pix, loc, rect});
        incrementStepCount();

    });

    timer->start(1000);

    setupWidgets();
    loadImage(colorPath, greyPath);
}

void MainWindowPuzzle::setupWidgets()
{
    // Set main window background
    this->setStyleSheet("QMainWindow { background-color: #2c3e50; }");

    // Central frame
    QFrame *centralFrame = new QFrame(this);
    centralFrame->setStyleSheet("QFrame { background: transparent; }");
    setCentralWidget(centralFrame);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralFrame);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(20);

    // ------------------ Left Panel ------------------
    QVBoxLayout *leftLayout = new QVBoxLayout;

    // Buttons
    QPushButton *backBtn = new QPushButton("Back");
    QPushButton *resetBtn = new QPushButton("Reset");
    QPushButton *undoBtn = new QPushButton("Undo");
    QString btnStyle =
        "QPushButton { padding: 10px; border-radius: 12px; background: #34495e; color: white; font-weight: bold; }"
        "QPushButton:hover { background: #3e5c76; }";
    backBtn->setStyleSheet(btnStyle);
    resetBtn->setStyleSheet(btnStyle);
    undoBtn->setStyleSheet(btnStyle);

    // Timer box
    timerLabel->setStyleSheet(
        "QLabel {"
        "background-color: rgba(0,0,0,180);"
        "color: white;"
        "font-weight: bold;"
        "font-size: 20px;"
        "padding: 10px;"
        "border-radius: 10px;"
        "min-width: 120px;"
        "text-align: center;"
        "}"
        );
    timerLabel->setAlignment(Qt::AlignCenter);

    // Puzzle pieces list inside a frame
    QFrame *piecesFrame = new QFrame;
    piecesFrame->setStyleSheet(
        "QFrame {"
        "background-color: rgba(255, 255, 255, 50);"
        "border-radius: 15px;"
        "padding: 10px;"
        "}"
        );
    QVBoxLayout *piecesLayout = new QVBoxLayout(piecesFrame);
    piecesLayout->addWidget(greyImage);
    piecesLayout->addWidget(piecesList);

    // Add timer and control buttons
    QPushButton *resetButton = new QPushButton("Reset");
    QPushButton *undoButton = new QPushButton("Undo");
    QPushButton *saveBtn = new QPushButton("Save");
    QPushButton *loadBtn = new QPushButton("Load");
    leftLayout->addWidget(saveBtn);
    leftLayout->addWidget(loadBtn);

    connect(saveBtn, &QPushButton::clicked, this, &MainWindowPuzzle::saveProgress);
    connect(loadBtn, &QPushButton::clicked, this, &MainWindowPuzzle::loadProgress);

    // Add widgets to left layout
    leftLayout->addWidget(backBtn);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(piecesFrame);
    leftLayout->addSpacing(20);
    QHBoxLayout *infoLayout = new QHBoxLayout();
    infoLayout->addWidget(timerLabel);
    infoLayout->addWidget(stepLabel); // ← Tambahkan stepLabel
    infoLayout->setSpacing(10);

    // ... dalam leftLayout, ganti addWidget(timerLabel) dengan:
    leftLayout->addLayout(infoLayout); // ← Gunakan layout yang berisi timer dan step
    leftLayout->addSpacing(20);
    leftLayout->addWidget(resetBtn);
    leftLayout->addWidget(undoBtn);
    leftLayout->addStretch();

    // ------------------ Right Panel ------------------
    puzzleBoardBox = new PuzzleBoardBox(gridSize);
    QVBoxLayout *rightLayout = new QVBoxLayout(puzzleBoardBox);
    rightLayout->setContentsMargins(5, 5, 5, 5);
    rightLayout->addWidget(puzzleWidget);

    // Add left and right panels to main layout
    mainLayout->addLayout(leftLayout, 0);   // left panel gets natural width
    mainLayout->addWidget(puzzleBoardBox, 1); // right panel expands

    // ------------------ Connections ------------------
    connect(backBtn, &QPushButton::clicked, this, &MainWindowPuzzle::close);
    connect(resetBtn, &QPushButton::clicked, this, &MainWindowPuzzle::resetPuzzle);
    connect(undoBtn, &QPushButton::clicked, this, &MainWindowPuzzle::undoMove);
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

void MainWindowPuzzle::setupPuzzle()
{
    if (puzzleWidget->width() <= 0 || puzzleWidget->height() <= 0) return;

    // Make puzzle widget square
    int boardSize = qMin(puzzleWidget->width(), puzzleWidget->height());
    puzzleWidget->setFixedSize(boardSize, boardSize);

    // Compute piece size
    int pieceSize = boardSize / gridSize;

    // Crop and scale puzzle image to fit exactly the grid
    int size = qMin(puzzleImage.width(), puzzleImage.height());
    puzzleImage = puzzleImage.copy((puzzleImage.width() - size)/2,
                                   (puzzleImage.height() - size)/2,
                                   size, size)
                      .scaled(pieceSize * gridSize,
                              pieceSize * gridSize,
                              Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);

    piecesList->clear();

    int rotations[4] = {0, 90, 180, 270};

    for(int y = 0; y < gridSize; ++y)
        for(int x = 0; x < gridSize; ++x)
        {
            QPixmap pieceImage = puzzleImage.copy(x*pieceSize, y*pieceSize, pieceSize, pieceSize);
            int rot = rotations[QRandomGenerator::global()->bounded(4)];
            piecesList->addPiece(pieceImage, QPoint(x, y), rot);
        }

    puzzleWidget->update();
}


void MainWindowPuzzle::resetPuzzle()
{
    puzzleWidget->clear();
    moveHistory.clear();
    moveCount = 0;
    stepLabel->setText(QString("Steps: %1").arg(moveCount));
    setupPuzzle();
}

void MainWindowPuzzle::undoMove()
{
    if(moveHistory.isEmpty()) return;
    Move last = moveHistory.takeLast();
    puzzleWidget->removePiece(last.rect);
    piecesList->addPiece(last.pixmap, last.location);
    puzzleWidget->update();
    moveCount--;
    stepLabel->setText(QString("Steps: %1").arg(moveCount));
}

void MainWindowPuzzle::setCompleted()
{
    timer->stop();
    QMessageBox::information(this, "Puzzle Complete", "Congratulations! Puzzle finished.");
    promptAndSaveRecord();
}

void MainWindowPuzzle::updateTimer()
{
    timeLeft--;
    timerLabel->setText(QString("Time: %1 s").arg(timeLeft));
    if(timeLeft <= 0)
    {
        timer->stop();
        QMessageBox::warning(this, "Time's up!", "Time is over!");
    }
}
void MainWindowPuzzle::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    if (puzzleWidget && puzzleBoardBox && !puzzleImage.isNull()) {
        // Make puzzle widget square based on the puzzleBoardBox
        int boardSize = qMin(puzzleBoardBox->width(), puzzleBoardBox->height());
        puzzleWidget->setFixedSize(boardSize, boardSize);
        setupPuzzle();
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
void MainWindowPuzzle::saveProgress() {
    bool ok;
    QString fileName = QInputDialog::getText(this, "Save Progress",
                                             "Enter a name for your save file:",
                                             QLineEdit::Normal, "", &ok);

    if (!ok || fileName.isEmpty()) {
        return; // User cancelled or entered empty name
    }

    // Remove any invalid characters from filename
    fileName = fileName.replace(QRegularExpression("[^a-zA-Z0-9_-]"), "_");

    QJsonObject saveData;
    saveData["gridSize"] = gridSize;
    saveData["timeLeft"] = timeLeft;
    saveData["gameMode"] = gameMode;    // Tambahkan metadata mode
    saveData["gameLevel"] = level;      // Tambahkan metadata level
    saveData["savedPieceSize"] = puzzleWidget->pieceSize();

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
    for (int i = 0; i < piecesList->count(); i++) {
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

    // Tambahkan metadata untuk tampilan di load
    saveData["saveName"] = fileName;
    saveData["timestamp"] = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // Tulis ke file
    QDir().mkpath("saves");
    QFile file("saves/" + fileName + ".json");
    if (file.open(QIODevice::WriteOnly)) {
        file.write(QJsonDocument(saveData).toJson());
        file.close();
        QMessageBox::information(this, "Saved", "Progress saved as: " + fileName);
    } else {
        QMessageBox::warning(this, "Error", "Could not save file: " + fileName);
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

    // Buat daftar nama file yang lebih user-friendly
    QStringList compatibleFiles;
    QMap<QString, QString> fileMap; // Maps display name to actual filename

    for (const QString &file : files) {
        QFile f(dir + "/" + file);
        if (f.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(f.readAll());
            QJsonObject data = doc.object();

            // Periksa kompatibilitas
            QString savedMode = data["gameMode"].toString();
            int savedLevel = data["gameLevel"].toInt();

            if (savedMode == gameMode && savedLevel == level) {
                QString saveName = data["saveName"].toString();
                QString timestamp = data["timestamp"].toString();

                QString displayName;
                if (!saveName.isEmpty()) {
                    displayName = QString("%1 (%2)").arg(saveName).arg(timestamp);
                } else {
                    displayName = QString("%1 (%2)").arg(file).arg(timestamp);
                }

                compatibleFiles.append(displayName);
                fileMap[displayName] = file;
            }
            f.close();
        }
    }

    if (compatibleFiles.isEmpty()) {
        QMessageBox::warning(this, "Load",
                             QString("No saved progress found for %1 mode level %2.")
                                 .arg(gameMode).arg(level));
        return;
    }

    bool ok;
    QString displayName = QInputDialog::getItem(this, "Load Progress",
                                                "Choose a save:", compatibleFiles, 0, false, &ok);

    if (!ok || displayName.isEmpty()) return;

    QString fileName = fileMap[displayName];
    QFile file(dir + "/" + fileName);

    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file: " + fileName);
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    QJsonObject saveData = doc.object();
    gridSize = saveData["gridSize"].toInt();
    timeLeft = saveData["timeLeft"].toInt();

    // Dapatkan pieceSize yang disimpan
    int savedPieceSize = saveData["savedPieceSize"].toInt();
    int currentPieceSize = puzzleWidget->pieceSize();

    // Hitung scaling factor
    double scaleFactor = (double)currentPieceSize / savedPieceSize;

    puzzleWidget->clear();
    piecesList->clear();

    // Load placed pieces
    QJsonArray placed = saveData["placedPieces"].toArray();
    for (auto v : placed) {
        QJsonObject obj = v.toObject();
        QPoint loc(obj["x"].toInt(), obj["y"].toInt());
        int rot = obj["rotation"].toInt();

        // Scale coordinates dari savedPieceSize ke currentPieceSize
        int rectX = obj["rectX"].toInt() * scaleFactor;
        int rectY = obj["rectY"].toInt() * scaleFactor;
        QRect rect(rectX, rectY, currentPieceSize, currentPieceSize);

        QPixmap pieceImg = puzzleImage.copy(loc.x() * savedPieceSize,
                                            loc.y() * savedPieceSize,
                                            savedPieceSize, savedPieceSize);

        // Scale image ke ukuran current
        pieceImg = pieceImg.scaled(currentPieceSize, currentPieceSize,
                                   Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

        puzzleWidget->addPieceWithRotation(pieceImg, loc, rot, rect); // ← Perlu modifikasi fungsi ini
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

    QMessageBox::information(this, "Loaded", "Progress loaded from: " + saveData["saveName"].toString());
}

void MainWindowPuzzle::incrementStepCount()
{
    moveCount++;
    stepLabel->setText(QString("Steps: %1").arg(moveCount));
}

