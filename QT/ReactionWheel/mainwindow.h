#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <string>
#include "reactionwheelplatform.hpp"
#include "serial.h"

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
    void sendAngle();
    void closeEvent(QCloseEvent *event);

private:
    ReactionWheelPlatform *rw;  // Project platform to grab data
    Ui::MainWindow *ui;
    void displayGyro();
    Serial *serial;
};

#endif // MAINWINDOW_H
