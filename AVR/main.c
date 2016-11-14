#include <stdio.h>
#include <avr/io.h>
#include "serial.h"
#include "motorDrive.h"
#include "pid.h"
#include "accel.h"
#include <util/delay.h> 
#include <stdint.h>
#include <math.h> 


int main(){
	pid_controller_t pid;

	pid_controller_init(&pid, 1.0f, 1.0f, 0);

	float set_point = 0.0f;
	float dt = 0.1f;
	float min = -255.0f;
	float max = 255.0f;
	float measure = 0.0f;
	/*
	float gyroAngle = 0.0f;
	float offset = 0.0f;
	float accelAngle = 0.0f;
	float ratio = 5.0f;
	*/

	initI2C();
	initUSART();
	initMotor();	
	bno055SetReg(BNO055_PWR_MODE_ADDR, 0); // Set power mode to read
	bno055SetReg(BNO055_OPR_MODE_ADDR, 8); // Set operationg mode to ALL imu data
			
	while(1){
	
	// Math notes - to be deleted when complete	
	// 16/lsb/deg/s
	// 1/16*(1/freq)* measure
		
		// Get gyro values
		uint8_t gyro_msbX = bno055ReadReg(BNO055_GYRO_DATA_X_MSB_ADDR);
		uint8_t accel_msbX = bno055ReadReg(BNO055_ACCEL_DATA_X_MSB_ADDR);
		uint8_t accel_msbY = bno055ReadReg(BNO055_ACCEL_DATA_Y_MSB_ADDR);
		
		// Get Euler value
		uint8_t euler_roll = bno055ReadReg(BNO055_EULER_R_MSB_ADDR);
		
		reportData(euler_roll, gyro_msbX, accel_msbX);
		/*
		accelAngle = tilt_anglecel_msbX, accel_msbY);
		// Get gyro data - offset is reading when standing still - ratio from data sheet.
		angGyro += (gyro_msbX - offset) * ratio;
		measure = gyroAngle * 0.7 + accelAngle * 0.3;
		*/
		
		measure = euler_roll;
		float error = set_point - measure;
		float control = pid_update(
			&pid,
         	error, 
        	0.0f,
        	dt, 
        	min, max,
        	min, max,
        	0.0f
        );
			
		_delay_ms(50);
		
	}
	
	return 0;
}
