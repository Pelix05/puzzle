// mainwindow.h
#pragma once
#include <QMainWindow>
#include <QPushButton>
#include "databasemanager.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    // Add this static function
    static void applyBackground(QWidget *widget, const QString &imagePath = ":/images/background.jpg");

private slots:
    void openDefaultPuzzleMenu();
    void openGeneratePuzzleMenu();

private:
    QPushButton* createGlassButton(const QString &text);
    DatabaseManager *dbManager;
};
