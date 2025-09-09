#ifndef MAINWINDOWPUZZLE_H
#define MAINWINDOWPUZZLE_H

#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "puzzle/pieceslist.h"
#include "puzzle/puzzlewidget.h"
#include "databasemanager.h"

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

    DatabaseManager *dbManager;
    int moveCount;
};


#endif // MAINWINDOWPUZZLE_H
