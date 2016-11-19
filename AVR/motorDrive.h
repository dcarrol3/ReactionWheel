// Motor controller

#ifndef motorDrive_h_
#define motorDrive_h_

#define PIN_NUM 0	// C pin number
#define CW 0
#define CCW 1

// Direction
extern uint8_t motorState;

// Initalize motor
void initMotor(void);
// Spin motor counter clockwise
void counterClockwise(void);
// Spin motor clockwise
void clockwise(void);
// Change motor speed 0 - 255
void setSpeed(uint8_t spd);
// Stop motor by reversing direction
void hardStop();

#endif
