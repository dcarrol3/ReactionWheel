#include <stdio.h>
#include <avr/io.h>
#include "accel.h"
#include "serial.h"
#include <util/delay.h> 
#include <stdint.h>    


void initI2C(void){
	TWBR = 32; // Set baud rate
	TWCR |= (1 << TWEN); // Enable TWI
}
// Wait for current communication to complete.
static void i2cWaitForComp(void){
	loop_until_bit_is_set(TWCR, TWINT);
}

// Start i2c communication.
static void start(void){
	TWCR = (_BV(TWINT)|_BV(TWEN)|_BV(TWSTA));
	i2cWaitForComp();
}

// Stop i2c communcation.
static void stop(void){
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWSTO));
}

// Send next byte of data.
static void send(uint8_t data){
	TWDR = data;
	TWCR = (_BV(TWINT) | _BV(TWEN));
	i2cWaitForComp();
}

// Read and send an acknowlegedment
static uint8_t readAcknowledge(void){
	TWCR = (_BV(TWINT) | _BV(TWEN) | _BV(TWEA));
	i2cWaitForComp();
	return (TWDR);
}

// Read and don't send an acknowlegedment
static uint8_t readNonAcknowledge(void){
	TWCR = (_BV(TWINT) | _BV(TWEN));
	i2cWaitForComp();
	return (TWDR);
}

// Set register value
void bno055SetReg(uint8_t reg, uint8_t data) {
	start();
	send(BNO055_ADDRESS_A << 1);
	send(reg);
	send(data);
	stop();
}

// Read register data from chip
uint8_t bno055ReadReg(uint8_t reg) {
	uint8_t tempByte;
	start();
	send(BNO055_ADDRESS_A << 1); // Send slave address
	send(reg);
	
	start();	
	send((BNO055_ADDRESS_A << 1) | 1); // Send slave address with read bit
	
	tempByte = readAcknowledge();
	
	readNonAcknowledge();
	stop();
	
	return tempByte;
}

// Send data to UART
void reportData(uint8_t x, uint8_t y, uint8_t z) {
	char str[128];
	// Convert to signed
	int8_t xVal = (int8_t)x;
	int8_t yVal = (int8_t)y;
	int8_t zVal = (int8_t)z;

	sendString("Gyro: ");
	sprintf(str, "%d, %d, %d\r\n", xVal, yVal, zVal);
	sendString(str);
}

int main(){
	initI2C();
	initUSART();	
	bno055SetReg(BNO055_PWR_MODE_ADDR, 0); // Set power mode to read
	bno055SetReg(BNO055_OPR_MODE_ADDR, 8); // Set operationg mode to ALL imu data

	while(1){
		
		// Get gyro values
		uint8_t gyro_msbX = bno055ReadReg(BNO055_GYRO_DATA_X_MSB_ADDR);
		uint8_t gyro_msbY = bno055ReadReg(BNO055_GYRO_DATA_Y_MSB_ADDR);
		uint8_t gyro_msbZ = bno055ReadReg(BNO055_GYRO_DATA_Z_MSB_ADDR);
		
	
		reportData(gyro_msbX, gyro_msbY, gyro_msbZ);
		
		_delay_ms(50);
		
	}
	
	return 0;
}





