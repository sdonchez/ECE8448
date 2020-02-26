// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 26 February 2020

/******************************************************************************
* main.c -	main routine and configuration subroutines for Homework 
*			Assignment 5, part 2. See README.md for assignment description.
******************************************************************************/
#include "address_map_arm.h"		// for register names

void disable_A9_interrupts (void);	// exceptions.c
void set_A9_IRQ_stack (void);		// exceptions.c
void config_GIC (void);				// exceptions.c
void config_KEYs (void);			// main.c
void config_HPS_timer(void);		// main.c
void enable_A9_interrupts (void);	// exceptions.c
void pushbutton_ISR(void);			// pushbutton_ISR.c
void timer0_ISR(void);				// timer0_ISR.c

int count = 0;						// to store the current value of the LEDs
int run = 1;						// enable or disable the counter
volatile int * LEDR_ptr = (int *) LEDR_BASE; // address of LEDs on HPS

/******************************************************************************
 * main: Configures the interrupts and associated hardware peripherals, then
 * loops forever writing the current value of count to the LEDs while waiting
 * for an interrupt from either the keys or the timer.
 *
 * @param	N/A
 * @returns	N/A
 * @note	Code provided with assignment, unmodified except for comments
 * @see		full description of program functionality in README.md
 *****************************************************************************/
int main(void)
{
	disable_A9_interrupts ();	// disable interrupts in the A9 processor
	set_A9_IRQ_stack ();		// initialize the stack pointer for IRQ mode
	config_GIC ();				// configure the general interrupt controller
	config_KEYs ();				// configure  KEYs to generate interrupts
	config_HPS_timer();			// confiugre HPS timer to generate interrupts

	enable_A9_interrupts ();	// enable interrupts in the A9 processor

	while (1) 					// wait for an interrupt
		*LEDR_ptr = count;		// assign current counter value to LEDs
} // main

/******************************************************************************
* config_KEYs: Setup the KEY interrupts in the FPGA 
*
* @param	N/A
* @returns	N/A
* @note		adding to pointers increments by number of registers, not # of bits
* @see		DE1 SoC Computer Manual
******************************************************************************/
void config_KEYs(void)
{
	volatile int * KEY_ptr = (int *) KEY_BASE;	// pushbutton KEY base address

	*(KEY_ptr + 2) = 0xF; 	// enable interrupts for the four KEYs
} // config_KEYs

/******************************************************************************
* config_HPS_timer: Setup the HPS Timer to function as required
*
* @param	N/A
* @returns	N/A
* @note		adding to pointers increments by number of registers, not # of bits
*			The timer control register (HPS_TIMER0_ptr + 2) performs the
*			following functions:
*			Bit 0:		Timer enable (Active High)
*			Bit 1:		Load initial value from LOAD register (HPS_TIMER0_ptr)
*			Bit 2:		Interrupt enable (Active Low)
*			Bit 3-31:	Unused
* @see		DE1 SoC Computer Manual
******************************************************************************/
void config_HPS_timer(void)
{
	volatile int * HPS_TIMER0_ptr = (int *) HPS_TIMER0_BASE; //Timer base addr.

	*(HPS_TIMER0_ptr + 2) = 0; 	// disable the timer

	// Load 25,000,000 into the register (1/4 second @ 100 MHz)
	*(HPS_TIMER0_ptr) = 25000000; 

	// enable interrupts for the timer
 	// enable starting from the value in the LOAD register
	// enable the timer	
	*(HPS_TIMER0_ptr + 2) = 0b011; 		
} // config_HPS_timer

