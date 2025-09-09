// mainwindow.h
#pragma once
#include <QMainWindow>
#include <QPushButton>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

    // Add this static function
    static void applyBackground(QWidget *widget, const QString &imagePath = ":/images/background.png");

private slots:
    void openDefaultPuzzleMenu();
    void openGeneratePuzzleMenu();

private:
    QPushButton* createGlassButton(const QString &text);
};
