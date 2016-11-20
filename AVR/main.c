#include <stdio.h>
#include <avr/io.h>
#include "serial.h"
#include "motorDrive.h"
#include "pid.h"
#include "accel.h"
#include <util/delay.h> 
#include <stdint.h>

// Max PWM - No idea why 253-255 do not work!
#define MAX_PWM 252.0f
// Center error
#define CENTER 0.0f
// Floating point absolute value
#define ABS(x) ((x) > 0 ? (x) : -(x)) 
// Chip ON/OFF
uint8_t runState = OFF;
int16_t eulerRoll = 0;

// Correct direction
uint8_t correctPWMValue(float val){
	// If pid is negative and wheel is still moving CW, change direction
	if(val > 0 && motorState == CW){
		counterClockwise();
	}
	else if(val < 0 && motorState == CCW){
		clockwise();
	}
	
	return (uint8_t)ABS(val);
}

int main(){
	pid_controller_t pid;
	
	// PID tuning. Params: Proportional - Integral - Derivative
	pid_controller_init(&pid, 200.0f, 1.2f, 0);

	float dt = 1.0f;
	float min = -MAX_PWM;
	float max = MAX_PWM;
	uint8_t spd = 0;
	float error = 0.0f;
	float control = 0.0f;
	int16_t prevEulerRoll = 0;

	initI2C();
	initUSART();
	initMotor();	
	bno055SetReg(BNO055_PWR_MODE_ADDR, 0); // Set power mode to read
	bno055SetReg(BNO055_OPR_MODE_ADDR, 8); // Set operating mode to ALL imu data
	
			
	while(1){
		
		if(runState == ON){
			// Get Euler LSB
			int16_t eulerRollLSB = bno055ReadReg(BNO055_EULER_R_LSB_ADDR);
			// Get Eurler MSb
			int16_t eulerRollMSB = bno055ReadReg(BNO055_EULER_R_MSB_ADDR);
			// Add MSB and LSB, divide by 16 to get degrees
			eulerRoll = ((eulerRollMSB << 8) | eulerRollLSB);
			
			// Gyro
			int8_t gyroRollMSB = bno055ReadReg(BNO055_GYRO_DATA_Z_LSB_ADDR);
			
			// Check for euler spikes
			eulerRoll = eulerRoll < 255 && eulerRoll > -255 ? eulerRoll : prevEulerRoll;
			
			reportData(eulerRoll, motorState, control);

			error = CENTER - ((float) eulerRoll / 16.0f);
			control = pid_update(
				&pid,
				error, 
				0.0f,
				dt, 
				min, max,
				min, max,
				0.0f
			);
			
			// Gyro adjustment
			
			control = control + ((float) gyroRollMSB * 0.0f);
			control = control > MAX_PWM ? MAX_PWM : control;
			control = control < -MAX_PWM ? -MAX_PWM : control;
			

			prevEulerRoll = eulerRoll;
			
			// Correct PWM value and update motor speed
			spd = correctPWMValue(control);
			setSpeed(spd);
        }
        else{
			setSpeed(30);
		}		
	}

	return 0;
}
