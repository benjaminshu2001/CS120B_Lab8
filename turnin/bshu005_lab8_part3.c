/*	Author: Benjamin Shu
 *  Partner(s) Name: 
 *	Lab Section: 22
 *	Assignment: Lab #8 Exercise #3
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

enum Sound_States {Start, Play, Stop} Sound_State;

unsigned count = 0; //accounts for position
double note[30] = {329.63, 523.25, 329.63, 493.88, 329.63, 440, 392, 349.23, 392, 261.63, 293.66, 349.23, 440, 523.25, 493.88, 392, 329.63, 293.66, 329.63, 293.66, 261.63, 246.94, -1};
double delay[30] = {200, 900, 200, 900, 400, 500, 350, 350, 1000, 400, 320, 325, 325, 325, 325, 325, 325, 325, 1000, 220, 200, 1000, 200};

void TckFctSound() {
	switch(Sound_State) {
		case Start:
			if((~PINA & 0x01) == 0x01) {
				Sound_State = Play;
			}
			else {
				Sound_State = Start;
			}
			break;
		case Play:
			if(note[count] != -1) {
				TimerSet(1000);
				Sound_State = Play;
			}
			else if(note[count] == -1 && (~PINA & 0x01) == 0x00) {
				Sound_State = Start;
			}
			else if(note[count] == -1 && (~PINA & 0x01) == 0x01) {
				count = 0;
				TimerSet(100);
				Sound_State = Stop;
			}
			else {
				Sound_State = Start;
			}
			break;
		case Stop:
			if((~PINA & 0x01) == 0x01) {
				Sound_State = Stop;
			}
			else {
				Sound_State = Start;
			}
			break;
	}
	switch(Sound_State) {
		case Start:
			set_PWM(0);
			count = 0;
			break;
		case Play:
			if(note[count] != -1) {
				TimerSet(delay[count]);
			}
			else {
				TimerSet(100);
			}
			set_PWM(note[count]);
			count++;
			break;
		case Stop:
			set_PWM(0);
			count = 0;
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
