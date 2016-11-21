#include "reactionwheelplatform.hpp"
#include <iostream>
#include <unistd.h>
#include <string>
#include "json.hpp"
#include "serial.h"


using namespace std;
using json = nlohmann::json;

int ReactionWheelPlatform::angle = 0;
int ReactionWheelPlatform::motorDir = 0;
int ReactionWheelPlatform::motorSpd = 0;
int ReactionWheelPlatform::setAngle = 0;

ReactionWheelPlatform::ReactionWheelPlatform()
{
    int errCheck = pthread_create(&dataThread, NULL, gatherData, NULL);
    if(errCheck){
        cout << "Error creating thread";
    }

}

// Data thread
void * ReactionWheelPlatform::gatherData(void*){
    string gyro;
    json j;
    int i = 0;
    struct stat buff;
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = DELAY_MS * 1000000;
    string port = "/dev/ttyUSB0";

    // Check which USB slot the device is in
    while(stat(port.c_str(), &buff) != 0 && i < 10){
        port[port.length() - 1] = (char)i + 48;
        i = i + 1;
    }
    cout << "FOUND: " << port << endl;
    Serial serial(port, 9600);

    while(1){
        try {
               gyro = serial.readLine();
               j = json::parse(gyro);
               angle = j["gyroX"];
               motorDir = j["gyroY"];
               motorSpd = j["gyroZ"];

           } catch(boost::system::system_error& e){
               cout << "UART not found?" << endl;
               cout << "Error: " << e.what() << endl;
           } catch(std::invalid_argument){
               cout << "Invalid json" << endl;
           }
        nanosleep(&time, &time);
    }
}

