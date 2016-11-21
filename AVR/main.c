#include <stdio.h>
#include <avr/io.h>
#include "serial.h"
#include "motorDrive.h"
#include "pid.h"
#include "accel.h"
#include <util/delay.h> 
#include <stdint.h>

// Max PWM - 253-255 do not work!
#define MAX_PWM 252.0f
// Floating point absolute value
#define ABS(x) ((x) > 0 ? (x) : -(x)) 
// Dead zone to avoid unnessecary oscillation
#define DEAD_ZONE_RANGE (5.0f)

// Chip ON/OFF
uint8_t runState = OFF;
// Position to move: -179 to +180
int32_t setAngle = 0;
uint8_t buffer[4];
volatile uint8_t dataDone = 0;


void updateRunState(int32_t data){
	if(data == ON){
		runState = ON;
	}
	else if(data == OFF) {
		runState = OFF;
	}
}

// Change set point
void updateSetAngle(){
	int32_t temp = 0;
	int32_t tempByte = 0;
	if(dataDone){
		// little endian
		for(uint8_t i = 0; i < 4; i++){
			tempByte = (int32_t) buffer[i];
			temp |= tempByte << (i * 8);
		}
		// If it's out of bounds don't change
		// Also update run state here as these must be out of bounds
		if(temp > 180 || temp < -179){
			updateRunState(temp);
		}
		else{
			setAngle = temp;
		}
		dataDone = 0;
	}
}

// Keep from spinning around


// Correct PID on the fly using gyroscope
void pidCorrection(int8_t gyro, float error, pid_controller_t *pid){
	float p_prime = 0.6f;
	float r = 0.0f;
	float err_prime = 0.0f;
	int8_t gyroMSB = ABS(gyro);
	
	// Make sure gyro reading is not 0 for calculations
	if(gyroMSB == 0) 
		gyroMSB = 1;
	
	// i correction - decreases integral with higher gyro values
	r = (10.0 / gyroMSB);
	pid->i = r * (0.35f);
	
	// p correction - increases p based on distance from set point
	err_prime = ABS(error);
	r = err_prime * (1.0 / 50.0f);
	r = r > 5.0f ? 5.0f : r;
	pid->p = r * p_prime;
}

// Correct direction
uint8_t correctPWMValue(float val){
	// Change direction if PID control passes 0 mark
	if(val < 0 && motorState == CW){
		counterClockwise();
	}
	else if(val > 0 && motorState == CCW){
		clockwise();
	}
	else if(val == 0){
		hardStop();
	}
	
	return (uint8_t)ABS(val);
}

int main(){
	pid_controller_t pid;
		
	// PID tuning. Params: Proportional - Integral - Derivative
	pid_controller_init(&pid, 1.5f, 1.0f, 0);

	float dt = 1.0f;
	float min = -MAX_PWM;
	float max = MAX_PWM;
	float error = 0.0f;
	float control = 0.0f;
	float angle = 0.0f;
	uint8_t spd = 0;
	int16_t eulerYaw = 0;
	int16_t eulerYawLSB = 0;
	int16_t eulerYawMSB = 0;

	initI2C();
	initUSART();
	initMotor();	
	bno055SetReg(BNO055_PWR_MODE_ADDR, 0); // Set power mode to read
	bno055SetReg(BNO055_OPR_MODE_ADDR, 8); // Set operating mode to ALL imu data
	
			
	while(1){
		
		if(runState == ON){
			// Get Euler LSB
			eulerYawLSB = bno055ReadReg(BNO055_EULER_H_LSB_ADDR);
			// Get Eurler MSb
			eulerYawMSB = bno055ReadReg(BNO055_EULER_H_MSB_ADDR);
			// Add MSB and LSB, divide by 16 to get degrees
			eulerYaw = ((eulerYawMSB << 8) | eulerYawLSB);
			// Gyro
			int8_t gyroMSB = bno055ReadReg(BNO055_GYRO_DATA_Z_MSB_ADDR);
			
			// Make yaw value go from -2880 to +2880 instead of 0 to 5760
			eulerYaw = eulerYaw > 2880 ? -(5760 - eulerYaw)  : eulerYaw;
			// Turn raw value into deg and subtract from set center
			angle = ((float) eulerYaw / 16.0f);
			error = ((float) setAngle) - angle;
			
			// Send data to desktop application			
			reportData(angle, motorState, control);
			
			// Correct PID on the fly
			pidCorrection(gyroMSB, error, &pid);
			
			if(ABS(error) > DEAD_ZONE_RANGE)
			{
				control = pid_update(
					&pid,
					error, 
					(float) eulerYaw,	// d
					dt, 
					min, max,
					-20, 20,	// Integral min/max
					0.0f
				);
			}
			
			// Gyro adjustment
			//control = control + ((float) gyroMSB * 0.0f);
			
			// Correct PWM value and update motor speed
			control = control > MAX_PWM ? MAX_PWM : control;
			control = control < -MAX_PWM ? -MAX_PWM : control;		
			spd = correctPWMValue(control);
			setSpeed(spd);
        }
        else{
			pid_controller_reset(&pid);
			//reportData(setAngle, motorState, control);
			setSpeed(0);
		}
		
		updateSetAngle();
		_delay_ms(1);		
	}

	return 0;
}
