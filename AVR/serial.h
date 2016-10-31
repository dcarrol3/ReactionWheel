/*
 * Doug Carroll
 * serial.h
 * Controls serial communication between AVR and USART terminal
 */

#ifndef serial_h_
#define serial_h_

#include <avr/io.h>

// Must be defined before includes.
// Unsigned long for cycles per second.
#define F_CPU 1000000UL 
// Communication rate in bits per second.
#define BAUD 9600

// Initialize the USART
void initUSART();
// Send a byte
void transmit(uint8_t data);
// Receive a byte
uint8_t receive();
// Receive a string (max 125 characters)
char * receiveString();
// Send a string
void sendString(const char str[]);


#endif
