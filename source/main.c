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
double note[9] = {261.63, 293.66, 329.63, 349.23, 392, 440, 493.88, 523.25, -1};

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
				Sound_State = Play;
			}
			else if(note[count] == -1 && (~PINA & 0x01) == 0x01) {
				count = 0;
				TimerSet(100);
				Sound_State = Stop;
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
			break;
		case Play:
			if(note[count] != 0) {
				TimerSet(500);
			}
			else {
				TimerSet(100);
			}
			set_PWM(note[count]);
			count++;
			break;
		case Stop:
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
