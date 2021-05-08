/*	Author: Benjamin Shu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #8 Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *  Link to Demo: https://www.youtube.com/watch?v=V5RrqMBu574
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "timer.h"
#include "pwm.h"

enum Sound_States {Start, Wait, C, D, E} Sound_State;

void TckFctSound() {
	switch(Sound_State) {
		case Start:
			Sound_State = Wait;
			break;
		case Wait:
			if((~PINA & 0x07) == 0x01) {
				Sound_State = C;
			}
			else if((~PINA & 0x07) == 0x02) {
				Sound_State = D;
			}
			else if((~PINA & 0x07) == 0x04) {
				Sound_State = E;
			}
			else {
				Sound_State = Wait;
			}
			break;
		case C:
			if((~PINA & 0x07) == 0x01) {
				Sound_State = C;
			}
			else {
				Sound_State = Wait;
			}
			break;
		case D:
			if((~PINA & 0x07) == 0x02) {
				Sound_State = D;
			}
			else {
				Sound_State = Wait;
			}
			break;
		case E:
			if((~PINA & 0x07) == 0x04) {
				Sound_State = E;
			}
			else {
				Sound_State = Wait;
			}
			break;
		default:
			Sound_State = Start;
			break;
	}
	switch(Sound_State) {
		case Start:
			break;
		case Wait:
			set_PWM(0);
			break;
		case C:
			set_PWM(261.63);
			break;
		case D:
			set_PWM(293.66);
			break;
		case E:
			set_PWM(329.63);
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
