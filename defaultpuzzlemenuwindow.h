#ifndef DEFAULTPUZZLEMENUWINDOW_H
#define DEFAULTPUZZLEMENUWINDOW_H

#pragma once
#include <QWidget>

class DefaultPuzzleMenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DefaultPuzzleMenuWindow(QWidget *parent = nullptr);

private slots:
    void startPuzzle3x3();
    void startPuzzle5x5();
    void startPuzzle7x7();
};


#endif // DEFAULTPUZZLEMENUWINDOW_H
