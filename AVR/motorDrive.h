#ifndef motorDrive_h_
#define motorDrive_h_

#define PIN_NUM 0	// C pin number

// Initalize motor
void initMotor(void);
// Spin motor counter clockwise
void counterClockwise(void);
// Spin motor clockwise
void clockwise(void);
// Change motor speed 0 - 255
void setSpeed(uint8_t spd);

#endif
