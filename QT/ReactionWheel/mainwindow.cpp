#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->xAxis->setSegmentStyle(QLCDNumber::Flat);
    ui->yAxis->setSegmentStyle(QLCDNumber::Flat);
    ui->zAxis->setSegmentStyle(QLCDNumber::Flat);
    rw = new ReactionWheelPlatform();
    connect(ui->startButton, SIGNAL (released()),this, SLOT (handleStart()));
    connect(ui->stopButton, SIGNAL (released()),this, SLOT (handleStop()));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete rw;
}

void MainWindow::update(){
    displayGyro();
}

void MainWindow::displayGyro(){
    ui->xAxis->display(ReactionWheelPlatform::gyroX);
    ui->yAxis->display(ReactionWheelPlatform::gyroY);
    ui->zAxis->display(ReactionWheelPlatform::gyroZ);
}

void MainWindow::handleStart(){
    // Start platform here
}

void MainWindow::handleStop(){
    // Stop platform here
}
