#include "mainwindowpuzzle.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
#include <QPushButton>
#include <QRandomGenerator>
#include <QResizeEvent>

////////////////////////////
// PuzzleBoardBox Implementation
////////////////////////////
void PuzzleBoardBox::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(QColor(255, 255, 255, 150));
    pen.setWidth(2);
    painter.setPen(pen);

    int w = width();
    int h = height();

    for (int i = 1; i < m_gridSize; ++i)
        painter.drawLine(i * w / m_gridSize, 0, i * w / m_gridSize, h);

    for (int i = 1; i < m_gridSize; ++i)
        painter.drawLine(0, i * h / m_gridSize, w, i * h / m_gridSize);
}

////////////////////////////
// MainWindowPuzzle Implementation
////////////////////////////
MainWindowPuzzle::MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int grid, int timerSeconds, DatabaseManager *db, QWidget *parent)
    : QMainWindow(parent), timeLeft(timerSeconds), gridSize(grid), dbManager(db), moveCount(0)
{
    // Set window size
    setMinimumSize(1700, 1000);         // force a large window
    setWindowState(Qt::WindowMaximized); // open maximized (optional)

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
    connect(timer, &QTimer::timeout, this, &MainWindowPuzzle::updateTimer);
    connect(puzzleWidget, &PuzzleWidget::puzzleCompleted, this, &MainWindowPuzzle::setCompleted);
    connect(puzzleWidget, &PuzzleWidget::piecePlaced, this, [this](QPixmap pix, QPoint loc, QRect rect){
        moveHistory.append({pix, loc, rect});
        moveCount++;
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

    // Add widgets to left layout
    leftLayout->addWidget(backBtn);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(piecesFrame);
    leftLayout->addSpacing(20);
    leftLayout->addWidget(timerLabel);
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

void MainWindowPuzzle::resetPuzzle()
{
    puzzleWidget->clear();
    moveHistory.clear();
    setupPuzzle();
}

void MainWindowPuzzle::undoMove()
{
    if(moveHistory.isEmpty()) return;
    Move last = moveHistory.takeLast();
    puzzleWidget->removePiece(last.rect);
    piecesList->addPiece(last.pixmap, last.location);
    puzzleWidget->update();
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

    // Resize puzzle board dynamically
    if (puzzleWidget && !puzzleImage.isNull()) {
        setupPuzzle();
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
    if(ok && !username.isEmpty())
    {
        int playerId = dbManager->insertPlayer(username);
        if(playerId == -1)
        {
            QSqlQuery query;
            query.prepare("SELECT player_id FROM players WHERE name = :name");
            query.bindValue(":name", username);
            query.exec();
            if(query.next()) playerId = query.value(0).toInt();
        }

        dbManager->insertRecord(playerId, duration, steps, level);
    }
}
