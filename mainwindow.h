#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "puzzle/databasemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
public:
    static void applyBackground(QWidget *widget);
    static void applyBackground(QWidget *widget, const QString &imagePath);


private:
    DatabaseManager *dbManager;

    QPushButton* createGlassButton(const QString &text);

    // Add these declarations:
    void openDefaultPuzzleMenu();
    void openGeneratePuzzleMenu();
};

#endif // MAINWINDOW_H
