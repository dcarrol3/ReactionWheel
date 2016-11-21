#ifndef REACTIONWHEELPLATFORM_H
#define REACTIONWHEELPLATFORM_H

#include <pthread.h>
#include <time.h>
#include <iostream>

#define DELAY_MS 50

class ReactionWheelPlatform
{
public:
    static int angle;
    static int motorDir;
    static int motorSpd;
    static int setAngle;
    ReactionWheelPlatform();

private:
    pthread_t dataThread;

    static void* gatherData(void*);
};

#endif // REACTIONWHEELPLATFORM_H
