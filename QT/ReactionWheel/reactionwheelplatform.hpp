#ifndef REACTIONWHEELPLATFORM_H
#define REACTIONWHEELPLATFORM_H

#include <pthread.h>
#include <time.h>
#include <iostream>

#define DELAY_MS 50

class ReactionWheelPlatform
{
public:
    static int gyroX;
    static int gyroY;
    static int gyroZ;
    ReactionWheelPlatform();

private:
    pthread_t dataThread;

    static void* gatherData(void*);
};

#endif // REACTIONWHEELPLATFORM_H
