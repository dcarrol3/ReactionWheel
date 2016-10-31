#ifndef REACTIONWHEELPLATFORM_H
#define REACTIONWHEELPLATFORM_H

#include <pthread.h>
#include <time.h>

#define DELAY_MS 50

class ReactionWheelPlatform
{
public:
    ReactionWheelPlatform();
    int getSpeed() const;
    int getCurrentAngle() const;

private:
    pthread_t dataThread;
    int currentAngle;
    int speed;
    static void* gatherData(void*);
};

#endif // REACTIONWHEELPLATFORM_H
