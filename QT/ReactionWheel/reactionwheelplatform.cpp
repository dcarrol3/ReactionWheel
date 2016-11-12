#include "reactionwheelplatform.hpp"
#include <iostream>
#include <unistd.h>
#include <string>
#include "json.hpp"
#include "serial.h"


using namespace std;
using json = nlohmann::json;

int ReactionWheelPlatform::gyroX = 0;
int ReactionWheelPlatform::gyroY = 0;
int ReactionWheelPlatform::gyroZ = 0;

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
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = DELAY_MS * 1000000;
    Serial serial("/dev/ttyUSB0", 9600);

    while(1){
        try {
               gyro = serial.readLine();
               j = json::parse(gyro);
               gyroX = j["gyroX"];
               gyroY = j["gyroY"];
               gyroZ = j["gyroZ"];

           } catch(boost::system::system_error& e)
           {
               cout<<"Error: "<<e.what()<<endl;
           }
        nanosleep(&time, &time);
    }
}

