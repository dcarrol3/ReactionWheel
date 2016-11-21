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
    ui->setAngleDisplay->setSegmentStyle(QLCDNumber::Flat);
    ui->pwmBar->setMaximum(252);
    ui->pwmBar->setValue(0);
    rw = new ReactionWheelPlatform();
    connect(ui->startButton, SIGNAL (released()),this, SLOT (handleStart()));
    connect(ui->stopButton, SIGNAL (released()),this, SLOT (handleStop()));
    connect(ui->sendAngleButton, SIGNAL (released()),this, SLOT (sendAngle()));
    // Check which USB slot the device is in
    int i = 0;
    struct stat buff;
    std::string port = "/dev/ttyUSB0";
    while(stat(port.c_str(), &buff) != 0 && i < 10){
        port[port.length() - 1] = (char)i + 48;
        i = i + 1;
    }
    serial = new Serial(port, 9600);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete rw;
    delete serial;
}

void MainWindow::update(){
    displayGyro();
}

void MainWindow::displayGyro(){
    ui->xAxis->display(ReactionWheelPlatform::angle);
    ui->yAxis->display(ReactionWheelPlatform::motorDir);
    ui->zAxis->display(ReactionWheelPlatform::motorSpd);
    ui->setAngleDisplay->display(ReactionWheelPlatform::setAngle);
    ui->pwmBar->setValue(qAbs(ReactionWheelPlatform::motorSpd));
}

void MainWindow::handleStart(){
    serial->writeInt(182);
    ui->powerLabel->setText("ON");
}

void MainWindow::handleStop(){
    serial->writeInt(181);
    ui->powerLabel->setText("OFF");
}

void MainWindow::sendAngle(){
    QString on = "ON";
    int angle = ui->angleBox->text().toInt();
    if(angle <= 180 && angle >= -179
            && ui->powerLabel->text() == on){
        ui->angleBox->setText("");
        ReactionWheelPlatform::setAngle = angle;
        serial->writeInt(angle);
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    handleStop();
    event->accept();
}
