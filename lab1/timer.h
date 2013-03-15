/*******************************************
*
* Header file for Timer stuff.
*
*******************************************/
#ifndef __TIMER_H
#define __TIMER_H

#include <inttypes.h> //gives us uintX_t

// number of empty for loops to eat up about 1 ms
// There are 36 or 37 instructions for each iteration of the loop.
// With a 20 MHz clock, the number of iterations ends up:
// 20,000,000 clocks/sec * 1 sec/1000 ms * 10ms * 1 iteration / 37 clock cycles
#define FOR_COUNT_10MS 5405

// This variable is made volatile so that the compiler doesn't optimize the
// loop out.
volatile uint32_t __ii;

// Assembly output of the following loop:
// 	sts __ii,__zero_reg__
// 	sts __ii+1,__zero_reg__
// 	sts __ii+2,__zero_reg__
// 	sts __ii+3,__zero_reg__
// 	rjmp .L3
// .L4:
// .LM5:
// 	lds r24,__ii               # Load Direct from SRAM, 2 clock cycles
// 	lds r25,__ii+1             # Load Direct from SRAM, 2 clock cycles
// 	lds r26,__ii+2             # Load Direct from SRAM, 2 clock cycles
// 	lds r27,__ii+3             # Load Direct from SRAM, 2 clock cycles
// 	adiw r24,1                 # Add Immediate to Word, 2 clock cycles
// 	adc r26,__zero_reg__       # Add with carry Two Registers, 1 clock cycle
// 	adc r27,__zero_reg__       # Add with carry Two Registers, 1 clock cycle
// 	sts __ii,r24               # Store Direct to SRAM, 2 cycles
// 	sts __ii+1,r25             # Store Direct to SRAM, 2 cycles
// 	sts __ii+2,r26             # Store Direct to SRAM, 2 cycles
// 	sts __ii+3,r27             # Store Direct to SRAM, 2 cycles
// .L3:
// .LM6:
// 	lds r24,__ii               # Load Direct from SRAM, 2 clock cycles
// 	lds r25,__ii+1             # Load Direct from SRAM, 2 clock cycles
// 	lds r26,__ii+2             # Load Direct from SRAM, 2 clock cycles
// 	lds r27,__ii+3             # Load Direct from SRAM, 2 clock cycles
// 	cpi r24,lo8(5405)          # Compare Register with Immediate, 1 cycle
// 	ldi r18,hi8(5405)          # Load Immediate, 1 cycle
// 	cpc r25,r18                # Compare with Carry, 1 cycle
// 	ldi r18,hlo8(5405)         # Load Immediate, 1 cycle
// 	cpc r26,r18                # Compare with Carry, 1 cycle
// 	ldi r18,hhi8(5405)         # Load Immediate, 1 cycle
// 	cpc r27,r18                # Compare with Carry, 1 cycle
// 	brlo .L4                   # Branch if Lower, 1 or 2 cycles
#define WAIT_10MS {for (__ii=0;__ii<FOR_COUNT_10MS; __ii++);}

#define G_TIMER_RESOLUTION 100
#define Y_TIMER_RESOLUTION 100

void init_timers();

#endif //__TIMER_H
