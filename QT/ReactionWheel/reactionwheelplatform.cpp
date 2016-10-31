#include "reactionwheelplatform.h"
#include <iostream>
#include <unistd.h>

using namespace std;

ReactionWheelPlatform::ReactionWheelPlatform()
{
    speed = 49;
    currentAngle = 0;
    int errCheck = pthread_create(&dataThread, NULL, gatherData, NULL);
    if(errCheck){
        cout << "Error creating thread";
    }

}

int ReactionWheelPlatform::getSpeed() const
{
    return speed;
}

int ReactionWheelPlatform::getCurrentAngle() const
{
    return currentAngle;
}

// Data thread
void * ReactionWheelPlatform::gatherData(void*){
    struct timespec time;
    time.tv_sec = 0;
    time.tv_nsec = DELAY_MS * 1000000;
    while(1){
        cout << "I'm alive!";
        nanosleep(&time, &time);
    }
}

