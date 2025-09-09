#ifndef GENERATEPUZZLEMENUWINDOW_H
#define GENERATEPUZZLEMENUWINDOW_H

#pragma once
#include <QWidget>
#include <QLabel>


class GeneratePuzzleMenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GeneratePuzzleMenuWindow(QWidget *parent = nullptr);

private slots:
    void chooseImage();
    void startCustomPuzzle();

private:
    QString selectedImagePath;
    int gridSize;
    int timerSeconds;
    QLabel *imagePreview;
};


#endif // GENERATEPUZZLEMENUWINDOW_H
