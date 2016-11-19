#include <avr/io.h>
#include "motorDrive.h"

uint8_t motorState = CW;

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
    TCCR0A |= (1 << WGM02) | (1 << WGM00);
    // Set prescaler to clock/8
    TCCR0B |= (1 << CS00);
    
    clockwise();
}

void counterClockwise(void){
	if(motorState == CW){
		motorState = CCW;
		PORTC |= (1 << PIN_NUM); // High
	}
}

void clockwise(void){
	if(motorState == CCW){
		motorState = CW;
		PORTC &= ~(1 << PIN_NUM); // Low
	}
}

void setSpeed(uint8_t spd){
	OCR0A = spd;
}

void hardStop(){
	if(motorState == CW){
		counterClockwise();
	}
	else{
		clockwise();
	}
	setSpeed(0);
}
