#ifndef MAINWINDOWPUZZLE_H
#define MAINWINDOWPUZZLE_H

#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include <QFrame>          // For PuzzleBoardBox
#include <Qpainter>
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
            "background-color: rgba(255, 255, 255, 50);" // semi-transparent
            "border-radius: 15px;"
            "}"
            );
    }



protected:
    void paintEvent(QPaintEvent *event) override {
        QFrame::paintEvent(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        QPen pen(QColor(255, 255, 255, 150)); // white, semi-transparent
        pen.setWidth(2);
        painter.setPen(pen);

        int w = width();
        int h = height();

        // Draw vertical lines
        for (int i = 1; i < m_gridSize; ++i) {
            int x = i * w / m_gridSize;
            painter.drawLine(x, 0, x, h);
        }

        // Draw horizontal lines
        for (int i = 1; i < m_gridSize; ++i) {
            int y = i * h / m_gridSize;
            painter.drawLine(0, y, w, y);
        }
    }




private:
    int m_gridSize;
};

class MainWindowPuzzle : public QMainWindow
{
    Q_OBJECT
public:
    MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int gridSize, int timerSeconds,  DatabaseManager *dbManager, QWidget *parent = nullptr);

    struct Move {
        QPixmap pixmap;
        QPoint location;
        QRect rect;
    };
    QVector<Move> moveHistory;

private slots:
    void setCompleted();
    void updateTimer();
    void resetPuzzle();
    void undoMove();
    void saveProgress();
    void loadProgress();


private:
    void loadImage(const QString &fileName, const QString &grey_fileName);
    void setupPuzzle();
    void setupWidgets();
    void promptAndSaveRecord();


    QPixmap puzzleImage;
    QLabel *greyImage;
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;
    QLabel *timerLabel;
    QTimer *timer;
    int timeLeft;
    int gridSize;

    PuzzleBoardBox *puzzleBoardBox;  // <-- New: box with grid overlay

    DatabaseManager *dbManager;
    int moveCount;
    int level;
};

#endif // MAINWINDOWPUZZLE_H
