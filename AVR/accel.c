#include <stdio.h>
#include <avr/io.h>
#include "serial.h"
#include "accel.h"
#include <util/delay.h> 
#include <stdint.h>
#include <math.h>   


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
void reportData(int16_t x, int16_t y, int16_t z) {
	char str[128];
	// Convert to signed
	int16_t xVal = (int16_t)x;
	int16_t yVal = (int16_t)y;
	int16_t zVal = (int16_t)z;

	sprintf(str, "{\"gyroX\": %d, \"gyroY\": %d, \"gyroZ\": %d}\r\n", xVal, yVal, zVal);
	sendString(str);
}

float tilt_angle(int8_t x, int8_t y){
	return atan2(x, y);
}







