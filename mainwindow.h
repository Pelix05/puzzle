// mainwindow.h
#pragma once
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void openDefaultPuzzleMenu();
    void openGeneratePuzzleMenu();

private:
    QPushButton* createGlassButton(const QString &text);  // Add this declaration
};
