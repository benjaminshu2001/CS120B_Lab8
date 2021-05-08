/*	Author: Benjamin Shu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #8 Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Link to Demo: https://www.youtube.com/watch?v=COIGBKj1VzU
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"
#include "pwm.h"

enum Sound_States {Start, Wait, Up, Down, Pressed, Off} Sound_State;

unsigned count = 0; //accounts for position
double note[8] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25};
unsigned char isOff = 0;

void TckFctSound() {
	switch(Sound_State) {
		case Start:
			Sound_State = Wait;
			break;
		case Wait:
			if((~PINA & 0x07) == 0x01) {
				Sound_State = Up;
			}
			else if((~PINA & 0x07) == 0x02) {
				Sound_State = Down;
			}
			else if((~PINA & 0x07) == 0x04) {
				Sound_State = Off;
			}
			else {
				Sound_State = Wait;
			}
			break;
		case Up:
			Sound_State = Pressed;
			break;
		case Down:
			Sound_State = Pressed;
			break;
		case Pressed:
			if((~PINA & 0x07) == 0x00) {
				Sound_State = Wait;
			}
			else {
				Sound_State = Pressed;
			}
			break;
		case Off:
			Sound_State = Pressed;
			break;
	}
	switch(Sound_State) {
		case Start:
			break;
		case Wait:
			break;
		case Up:
			if(count < 7) {
				count++;
			}
			if(isOff) {
				set_PWM(note[count]);
			}
			break;
		case Down:
			if(count > 0) {
				count--;
			}
			if(isOff) {
				set_PWM(note[count]);
			}
			break;
		case Pressed:
			break;
		case Off:
			if(isOff == 0) {
				isOff = 1;
				set_PWM(note[count]);
			}
			else {
				isOff = 0;
				set_PWM(0);
			}

			break;
		default:
			set_PWM(0);
			break;
	}
}

				
int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    /* Insert your solution below */
	PWM_on();
	TimerSet(200);
	TimerOn();
    while (1) {
		TckFctSound();
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}
