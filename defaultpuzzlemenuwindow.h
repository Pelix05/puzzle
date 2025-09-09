#ifndef DEFAULTPUZZLEMENUWINDOW_H
#define DEFAULTPUZZLEMENUWINDOW_H

#pragma once
#include"databasemanager.h"
#include <QWidget>

class DefaultPuzzleMenuWindow : public QWidget
{
    Q_OBJECT
public:
    explicit DefaultPuzzleMenuWindow(DatabaseManager *db);

private slots:
    void startPuzzle3x3();
    void startPuzzle5x5();
    void startPuzzle7x7();

private :
    DatabaseManager *dbManager;
};



#endif // DEFAULTPUZZLEMENUWINDOW_H
