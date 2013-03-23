#include "timer.h"
#include "LEDs.h"

#include <stdio.h>
#include <avr/interrupt.h>

// GLOBALS
extern uint32_t G_green_ticks;
extern uint32_t G_yellow_ticks;
extern uint32_t G_ms_ticks;

extern uint16_t G_red_period;
extern uint16_t G_green_period;
extern uint16_t G_yellow_period;

extern uint16_t G_release_red;

void init_timers() {
	// -------------------------  RED --------------------------------------//
	// Software Clock Using Timer/Counter 0.
	// THE ISR for this is below.

	// SET appropriate bits in TCCR....

	// Using CTC mode with OCR0 for TOP. This is mode X, thus WGM0/1/0 = .
	TCCR0A &= ~(1 << WGM00);
	TCCR0A |= (1 << WGM01);
	TCCR0B &= ~(1 << WGM02);

	// Using pre-scaler 256. This is CS0/1/0/0 = 
	TCCR0B |= (1 << CS02);
	TCCR0B &= ~(1 << CS01);
	TCCR0B &= ~(1 << CS00);
	
	// Software Clock Interrupt Frequency: 1000 = f_IO / (prescaler*(OCR0+1))
	// Set OCR0 appropriately for TOP to generate desired frequency of 1KHz
	printf("Initializing software clock to freq 1000Hz (period 1 ms)\n");	
	OCR0A = 77;

	//Enable output compare match interrupt on timer 0A
	TIMSK0 |= (1 << OCIE0A);

	// Initialize counter
	G_ms_ticks = 0;

	//--------------------------- YELLOW ----------------------------------//
	// Set-up of interrupt for toggling yellow LEDs. 
	// This task is "self-scheduled" in that it runs inside the ISR that is 
	// generated from a COMPARE MATCH of 
	//      Timer/Counter 3 to OCR3A.
	// Obviously, we could use a single timer to schedule everything, but we are experimenting here!
	// THE ISR for this is in the LEDs.c file

	// This timer should have a period of 100ms
	// Software Clock Interrupt Frequency: 10 = f_IO / (prescaler*(OCR3+1))

	// Using CTC mode with OCR3 for TOP. WGMn3/2/1/0 to 0/1/0/0
	TCCR3B &= ~(1 << WGM33);
	TCCR3B |=  (1 << WGM32);
	TCCR3A &= ~(1 << WGM31);
	TCCR3A &= ~(1 << WGM30);

	// Using pre-scaler 64. This is CS0/1/0/0 = 0/1/1
	// Set the prescaler to 64 and OCRn0 to 31,249
	TCCR3B &= ~(1 << CS32);
	TCCR3B |=  (1 << CS31);
	TCCR3B |=  (1 << CS30);

	// Set TOP value to 31249
	OCR3A = 31249;

	//	printf("Initializing yellow clock to freq %dHz (period %d ms)\n",(int)(10),Y_TIMER_RESOLUTION);	

	//Enable output compare match interrupt on timer 3A
	TIMSK3 |= (1 << OCIE3A);

	G_yellow_ticks = 0;

	//--------------------------- GREEN ----------------------------------//
	// Set-up of interrupt for toggling green LED. 
	// This "task" is implemented in hardware, because the OC1A pin will be toggled when 
	// a COMPARE MATCH is made of 
	//      Timer/Counter 1 to OCR1A.
	// We will keep track of the number of matches (thus toggles) inside the ISR (in the LEDs.c file)
	// Limits are being placed on the frequency because the frequency of the clock
	
	// Using CTC mode with OCR3 for TOP. WGMn3/2/1/0 to 0/1/0/0
	TCCR1B &= ~(1 << WGM13);
	TCCR1B |=  (1 << WGM12);
	TCCR1A &= ~(1 << WGM11);
	TCCR1A &= ~(1 << WGM10);

	// Using pre-scaler 64. This is CS1/2/1/0 = 1/0/1
	// Set the prescaler to 64 and OCRn0 to 31,249
	TCCR1B |=  (1 << CS12);
	TCCR1B &= ~(1 << CS11);
	TCCR1B |=  (1 << CS10);

	// Toggle OC1A on Compare Match
	TCCR1A &= ~(1 << COM1A1);
	TCCR1A |=  (1 << COM1A0);

	// Interrupt Frequency: 1/G_green_period = f_IO / (1024*OCR1A)
	// Set OCR1A appropriately for TOP to generate desired frequency.
	// NOTE: This IS the toggle frequency.
	printf("green period %d\n",G_green_period);
	OCR1A = (uint16_t)((float)(20000000.0 / (1000.0*1024)) * G_green_period);

	printf("Set OCR1A to %d\n",OCR1A);
	printf("Initializing green clock to freq %dHz (period %d ms)\n",(int)(1000/G_green_period),G_green_period);

	// A match to this will toggle the green LED.
	// Regardless of its value (provided it is less than OCR1A), it will match at the frequency of timer 1.
	OCR1B = 1;

	//Enable output compare match interrupt on timer 1B
	TIMSK1 |= (1 << OCIE1B);
}

//INTERRUPT HANDLERS
ISR(TIMER0_COMPA_vect) {

	// This is the Interrupt Service Routine for Timer0 (10ms clock used for scheduling red).
	// Each time the TCNT count is equal to the OCR0 register, this interrupt is "fired".

	// Increment ticks
	G_ms_ticks++;

	// if time to toggle the RED LED, set flag to release
	if ( ( G_ms_ticks % G_red_period ) == 0 ) {
		G_release_red = 1;
	}
}
