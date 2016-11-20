#include <avr/io.h>
#include "motorDrive.h"

uint8_t motorState = CW;

void initMotor(void){
	// Enable PWM output on D5 and D6
    DDRD |= (1 << DDD6) | (1 << DDD5);
	// PWM to 0% duty cycle initally
	OCR0A = 0;
	OCR0B = 0;
	// Enable non-inverting mode
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
	// Enable phase-corrected pwm mode
    TCCR0A |= (1 << WGM02) | (1 << WGM00);
    // Set prescaler to clock/8
    TCCR0B |= (1 << CS00);
    
    clockwise();
}

void counterClockwise(void){
	if(motorState == CW){
		motorState = CCW;
	}
}

void clockwise(void){
	if(motorState == CCW){
		motorState = CW;
	}
}

void setSpeed(uint8_t spd){
	if(motorState == CW){
		OCR0B = 0;
		OCR0A = spd;
	}
	else{
		OCR0A = 0;
		OCR0B = spd;
	}
}

void hardStop(){
	OCR0A = 0;
	OCR0B = 0;
}
