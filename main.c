/*	Author: gturr001, Giovany Turrubiartes
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #8  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum SM1_States { SM1_Start, SM1_ON, SM1_Wait1, SM1_Wait2} SM1_State;
enum SM2_States { SM2_Start, SM2_Up, SM2_Down, SM2_WaitUp, SM2_WaitDown, SM2_Wait} SM2_State;
static unsigned button1 = 0x00;
static unsigned button2 = 0x00;
static unsigned button3 = 0x00;

static int currVal = 0;
static double noteArray[8] = {261.63, 293.63, 329.63, 349.23, 392.00, 440.00, 493.33, 523.25};


void set_PWM(double frequency) {
	static double current_frequency;

	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } 
		else { TCCR3B |= 0x03; }

		if (frequency < 0.954) {
			OCR3A = 0xFFFF;
		}

		else if (frequency > 31250) {
			OCR3A = 0x0000;
		}

		else {
			OCR3A = (short)(8000000 / (128 * frequency)) - 1; 
		}

		TCNT3 = 0;
		current_frequency = frequency;

	}
}

void PWM_on () {
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32);

	set_PWM(noteArray[currVal]);

}
void PWM_off () {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}


void OnOff() {
	switch (SM1_State) { 
		case SM1_Start:
			if (button1 && (!button2 && !button3)) {
				SM1_State = SM1_ON;
			}

			else if (!button1) {
				SM1_State = SM1_Start;
			}

			break;

		case SM1_ON:
			SM1_State = SM1_Wait1;
			break;

		case SM1_Wait1:
			if (!button1) {
				SM1_State = SM1_Wait2;
			}

			else if (button1) {
				SM1_State = SM1_Wait1;
			}

			break;

		case SM1_Wait2:
			if(button1) {
				SM1_State = SM1_Start;
			}

			else if (!button1) {
				SM1_State = SM1_Wait2;
			}

			break;

		default:
			SM1_State = SM1_Start;
			break;
	}

	
	switch(SM1_State) {
		case SM1_Start:
			PWM_off();
			break;

		case SM1_ON:
			PWM_on();
			break;

		case SM1_Wait1:
			break;

		case SM1_Wait2:
			break;


	}
			

}

void UpDown() {
//	static double noteArray [8] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00,493.33, 523.25};
//	static int currVal = 0;

	switch (SM2_State) {
		case SM2_Start:
			if (button2) {
				SM2_State = SM2_Up;
			}

			else if (button3) {
				SM2_State = SM2_Down;
			}

			else {
				SM2_State = SM2_Start;
			}

			break;

		case SM2_Up:

			SM2_State = SM2_WaitUp;
			break;

		case SM2_Down:

			SM2_State = SM2_WaitDown;
			break;


		case SM2_WaitUp:

			if (button2) {
				SM2_State = SM2_WaitUp;
			}

			else if (!button2) {
				SM2_State = SM2_Wait;
			}
			
			break;

		case SM2_WaitDown:

			if (button3) {
				SM2_State = SM2_WaitDown;
			}

			else if (!button2) {
				SM2_State = SM2_Wait;
			}

			break;

		case SM2_Wait:

			if (button2) {
				SM2_State = SM2_Up;
			}

			else if (button3) {
				SM2_State = SM2_Down;
			}

			else {
				SM2_State = SM2_Wait;
			}

			break;

		default:
			SM2_State = SM2_Start;

			break;

	}


	switch (SM2_State) {
		case SM2_Start:
			currVal = -1;
			break;

		case SM2_Up:
			if (currVal < 7) {
				currVal = currVal + 1;
				set_PWM(noteArray[currVal]);

			}


			break;

		case SM2_Down:
			if (currVal > 0) {
				currVal = currVal - 1;
				set_PWM(noteArray[currVal]);
			}

			break;

		case SM2_WaitUp:
			break;

		case SM2_WaitDown:
			break;

		case SM2_Wait:
			break;


	}
			
}



int main(void) {
	DDRA = 0x00;
	PORTA = 0xFF;
	DDRB = 0x40; 
	PORTB = 0x00;


//	PWM_on();
		
    /* Insert your solution below */
    while (1) {

	    button1 = ~PINA & 0x01;
	    button2 = ~PINA & 0x02;
	    button3 = ~PINA & 0x04;

	    OnOff();
	    UpDown();
	    

    }
}
