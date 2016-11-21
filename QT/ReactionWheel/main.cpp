#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <time.h>
#include "serial.h"

using namespace std;
using namespace boost;

int main(int argc, char *argv[])
{

    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = DELAY_MS * 1000000;
    QApplication a(argc, argv);
    MainWindow w;

    w.setAttribute(Qt::WA_QuitOnClose);
    w.show();

    // GUI loop
    while(1){
        w.update();
        QCoreApplication::processEvents();
        nanosleep(&time, &time);
   }


    return a.exec();
}
