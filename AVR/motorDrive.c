#include <avr/io.h>
#include "motorDrive.h"

void initMotor(void){
	// Enable output pin for direction
	DDRC |= (1 << DDC0);
	// Enable PWM output on D6
    DDRD |= (1 << DDD6);
	// PWM to 0% duty cycle initally
	OCR0A = 0;
	// Enable non-inverting mode
	TCCR0A |= (1 << COM0A1);
	// Enable phase-corrected pwm mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00);
    // Set prescaler to clock/8
    TCCR0B |= (1 << CS01);
}

void counterClockwise(void){
	// High
	PORTC |= (1 << PIN_NUM);
}

void clockwise(void){
	// Low
	PORTC &= ~(1 << PIN_NUM);
}

void setSpeed(uint8_t spd){
	OCR0A = spd;
}
