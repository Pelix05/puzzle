#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include "databasemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    static void applyBackground(QWidget *widget, const QString &imagePath = ":/images/background.jpg");

private:
    DatabaseManager *dbManager;

    QPushButton* createGlassButton(const QString &text, int width, int height, int fontSize);

    // Add these declarations:
    void openDefaultPuzzleMenu();
    void openGeneratePuzzleMenu();
};

#endif // MAINWINDOW_H
