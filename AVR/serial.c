/*
 * Doug Carroll
 * serial.c
 */

#include "serial.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <util/delay.h>     
#include <util/setbaud.h>
#include <string.h>

#define STRING_LEN 128
uint8_t runTime = 0;

// Receive interrupt - UDR0 is data byte
ISR(USART_RX_vect){
	uint8_t data = UDR0;
	dataDone = 1;
	runTime = runTime + 1;
	if(runTime == 4){
		runTime = 0;
		dataDone = 1;
		buffer[3] = data;	
	}
	else{
		buffer[runTime - 1] = data;	
	}
	
}

void initUSART(){
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	// Use double speed mode or not - Load UCSRnA register.
	#if USE_2X
	UCSR0A |= (1 << U2X0);
	#else
	UCSR0A &= ~(1 << U2X0);
	#endif
	
	// Transmit enable - Load UCSRnB register - enable interrupt on receive
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1<<RXCIE0);
	// USART mode settings - Load UCSRnC register.
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
	
	// Enable interrupt
	sei();
}

void transmit(uint8_t data){
	loop_until_bit_is_set(UCSR0A, UDRE0);
	// Set data to register for sending.
	UDR0 = data;
}

uint8_t receive(){
	loop_until_bit_is_set(UCSR0A, RXC0);
	// Return contents of data register.
	return UDR0;
}

// Get string stopped by new line character
char * receiveString(){
	char str[STRING_LEN];
	char *ptr = str;
	char c;
	uint8_t i = 0;
	
	while(((c = receive()) != '\r') && i < STRING_LEN - 3){
		str[i] = c;
		str[i + 1] = '\r';
		str[i + 2] = '\n';
		str[i + 3] = '\0';
		i++;
	}
	// Jump to new line
	transmit('\r');
	transmit('\n');
	
	return ptr;
}

// Send data as JSON through UART
void reportData(int16_t x, int16_t y, int16_t z) {
	char str[128];
	// Convert to signed
	int16_t xVal = (int16_t)x;
	int16_t yVal = (int16_t)y;
	int16_t zVal = (int16_t)z;

	sprintf(str, "{\"gyroX\": %d, \"gyroY\": %d, \"gyroZ\": %d}\r\n", xVal, yVal, zVal);
	sendString(str);
}

void sendString(const char str[]){
	uint8_t i = 0;
	// While character is not null
	while(str[i]){
		transmit(str[i]);
		i++;
	}
}

