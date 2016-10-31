#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reactionwheelplatform.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void update();  // Update the windows data

private slots:
    void handleStart();
    void handleStop();

private:
    ReactionWheelPlatform *rw;  // Project platform to grab data
    Ui::MainWindow *ui;
    void displayMotorSpeed();
    void displayDirection();
    void displaySetDirection();
};

#endif // MAINWINDOW_H
