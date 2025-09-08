#ifndef MAINWINDOWPUZZLE_H
#define MAINWINDOWPUZZLE_H

#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "puzzle/pieceslist.h"
#include "puzzle/puzzlewidget.h"

class MainWindowPuzzle : public QMainWindow
{
    Q_OBJECT
public:
    MainWindowPuzzle(const QString &colorPath, const QString &greyPath, int gridSize, int timerSeconds, QWidget *parent = nullptr);

private slots:
    void setCompleted();
    void updateTimer();

private:
    void loadImage(const QString &fileName, const QString &grey_fileName);
    void setupPuzzle();
    void setupWidgets();

    QPixmap puzzleImage;
    QLabel *greyImage;
    PiecesList *piecesList;
    PuzzleWidget *puzzleWidget;
    QLabel *timerLabel;
    QTimer *timer;
    int timeLeft;
    int gridSize;
};


#endif // MAINWINDOWPUZZLE_H
