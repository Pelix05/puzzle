#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void openDefaultPuzzleMenu();
    void openGeneratePuzzleMenu();
};
