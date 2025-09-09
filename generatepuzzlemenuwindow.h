#ifndef GENERATEPUZZLEMENUWINDOW_H
#define GENERATEPUZZLEMENUWINDOW_H

#pragma once
#include <QWidget>
#include <QLabel>
#include "databasemanager.h"


class GeneratePuzzleMenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit GeneratePuzzleMenuWindow(DatabaseManager* db, QWidget *parent = nullptr);

private slots:
    void chooseImage();
    void startCustomPuzzle();

private:
    DatabaseManager *dbManager;
    QString selectedImagePath;
    int gridSize;
    int timerSeconds;
    QLabel *imagePreview;
};


#endif // GENERATEPUZZLEMENUWINDOW_H
