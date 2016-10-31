#include <iostream>
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->motorValue->setSegmentStyle(QLCDNumber::Flat);
    ui->currentDir->setSegmentStyle(QLCDNumber::Flat);
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
    displayMotorSpeed();
    displayDirection();
}

void MainWindow::displayMotorSpeed(){
    ui->motorValue->display(rw->getSpeed());
}

void MainWindow::displayDirection(){
    ui->currentDir->display(rw->getCurrentAngle());
}

void MainWindow::handleStart(){
    // Start platform here
}

void MainWindow::handleStop(){
    // Stop platform here
}
