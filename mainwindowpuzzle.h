#ifndef MAINWINDOWPUZZLE_H
#define MAINWINDOWPUZZLE_H

#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QFrame>
#include <QPixmap>
#include <QResizeEvent>
#include "puzzle/pieceslist.h"
#include "puzzle/puzzlewidget.h"
#include "puzzle/databasemanager.h"

class PuzzleBoardBox : public QFrame {
    Q_OBJECT
public:
    explicit PuzzleBoardBox(int gridSize, QWidget *parent = nullptr)
        : QFrame(parent), m_gridSize(gridSize)
    {
        setStyleSheet(
            "QFrame {"
            "background-color: rgba(255, 255, 255, 50);"
            "border-radius: 15px;"
            "}"
            );
    }



protected:
    void paintEvent(QPaintEvent *event) override;




private:
    int m_gridSize;

};

class MainWindowPuzzle : public QMainWindow
{
    Q_OBJECT
public:
    MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int gridSize,
                     int timerSeconds, DatabaseManager *dbManager,const QString &mode, QWidget *parent = nullptr);

    struct Move {
        QPixmap pixmap;
        QPoint location;
        QRect rect;
    };
    QVector<Move> moveHistory;

protected:
    void resizeEvent(QResizeEvent *event) override;  // <--- added for dynamic resizing

private slots:
    void setCompleted();
    void updateTimer();
    void resetPuzzle();
    void undoMove();
    void saveProgress();
    void loadProgress();

public slots:
    void incrementStepCount();

private:
    void loadImage(const QString &fileName, const QString &grey_fileName);
    void setupPuzzle();
    void setupWidgets();
    void promptAndSaveRecord();

    QString gameMode;


    QPixmap puzzleImage;
    QLabel *greyImage;
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;
    QLabel *timerLabel;
    QTimer *timer;
    int timeLeft;
    int gridSize;

    PuzzleBoardBox *puzzleBoardBox;
    int moveCount;
    QLabel *stepLabel;

    DatabaseManager *dbManager;
    int level;
};

#endif // MAINWINDOWPUZZLE_H
