#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "reactionwheelplatform.hpp"

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
    void displayGyro();
};

#endif // MAINWINDOW_H
