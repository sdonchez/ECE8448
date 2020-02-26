// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 26 February 2020

/******************************************************************************
* exceptions.c -	subroutines for configuring and handling interrupts for 
*					Homework Assignment 5, Part 1. See README.md for assignment
*					description.
*
* This file:
* 1. defines exception vectors for the A9 processor
* 2. provides code that sets the IRQ mode stack, and that dis/enables interrupts
* 3. provides code that initializes the generic interrupt controller
*
* Indicated portions of this file are unmodified, original taken from Intel 
* FPGA monitor program interrupt sample program
******************************************************************************/

#include "address_map_arm.h"		//for register names
#include "interrupt_ID.h"			//for IRQ IDs
#include "defines.h"				//for other SoC definitions

void pushbutton_ISR (void);			//pushbutton_ISR.c
void timer0_ISR (void);				//timer0_ISR.c
void config_interrupt (int, int);	//exceptions.c


/******************************************************************************
 * __cs3_isr_irq: IRQ exception handler - determines what the source of an
 *				  interrupt is and calls the appropriate ISR, handles clearing
 *				  the GIC interrupt notification.
 *
 * @param	N/A
 * @returns	N/A
 * @note	with the exception of lines 52-53, code is unmodified (see header)
 * @see		N/A
 *****************************************************************************/							
void __attribute__ ((interrupt)) __cs3_isr_irq (void)
{
	// Read the ICCIAR from the CPU interface in the GIC
	int address;
	int interrupt_ID;
	
	address = MPCORE_GIC_CPUIF + ICCIAR;
	interrupt_ID = *(int *)address;
   
	if (interrupt_ID == KEYS_IRQ)		// check if interrupt is from the KEYs
		pushbutton_ISR ();

	else if (interrupt_ID == HPS_TIMER0_IRQ) // check if interrupt from timer0 
		timer0_ISR ();

	else
		while (1);							// if unexpected, then stay here

	// Write to the End of Interrupt Register (ICCEOIR)
	address = MPCORE_GIC_CPUIF + ICCEOIR;
	*(int *)address = interrupt_ID;

	return;
} // __cs3_isr_irq

/******************************************************************************
 * Additional Exception Handlers: provided, unmodified
 *
 * @param	N/A
 * @returns	N/A
 * @note	code is unmodified (see header)
 * @see		N/A
 *****************************************************************************/ 
void __attribute__ ((interrupt)) __cs3_reset (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_undef (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_swi (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_pabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_dabort (void)
{
    while(1);
}

void __attribute__ ((interrupt)) __cs3_isr_fiq (void)
{
    while(1);
}
/******************************************************************************
End additional exception handlers
******************************************************************************/

/******************************************************************************
 * disable_A9_interrupts: turn off interrupts so we can configure the GIC
 *
 * @param	N/A
 * @returns	N/A
 * @note	code is unmodified (see header)
 * @see		N/A
 *****************************************************************************/
void disable_A9_interrupts(void)
{
	int status = 0b11010011;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
}// disable_A9_interrupts

/******************************************************************************
 * set_A9_IRQ_stack: Initialize the banked stack pointer register for IRQ mode
 *
 * @param	N/A
 * @returns	N/A
 * @note	code is unmodified (see header)
 * @see		N/A
 *****************************************************************************/
void set_A9_IRQ_stack(void)
{
	int stack, mode;
	stack = A9_ONCHIP_END - 7;	// top of A9 onchip memory, aligned to 8 bytes
	/* change processor to IRQ mode with interrupts disabled */
	mode = INT_DISABLE | IRQ_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
	/* set banked stack pointer */
	asm("mov sp, %[ps]" : : [ps] "r" (stack));

	/* go back to SVC mode before executing subroutine return! */
	mode = INT_DISABLE | SVC_MODE;
	asm("msr cpsr, %[ps]" : : [ps] "r" (mode));
} //set_A9_IRQ_stack

/******************************************************************************
 * enable_A9_interrupts: turn on interrupts once configuration is complete
 *
 * @param	N/A
 * @returns	N/A
 * @note	code is unmodified (see header)
 * @see		N/A
 *****************************************************************************/
void enable_A9_interrupts(void)
{
	int status = SVC_MODE | INT_ENABLE;
	asm("msr cpsr, %[ps]" : : [ps]"r"(status));
} //enable_A9_interrupts

/******************************************************************************
 * config_GIC: Configure the Generic Interrupt Controller (GIC) by enabling
 *             both interrupts that will be used
 *
 * @param	N/A
 * @returns	N/A
 * @note	code is unmodified with the exception of line 169 (see header)
 * @see		N/A
 *****************************************************************************/
void config_GIC(void)
{
	int address;
  	config_interrupt (KEYS_IRQ, CPU0); 	// configure the FPGA KEYs interrupt
	config_interrupt (HPS_TIMER0_IRQ, CPU0); //configre the Timer0 Interrupt
    
  	// Set Interrupt Priority Mask Register (ICCPMR). 
    // Enable interrupts of all priorities 
	address = MPCORE_GIC_CPUIF + ICCPMR;
  	*(int *) address = 0xFFFF;       

  	// Set CPU Interface Control Register (ICCICR). 
    // Enable signaling of interrupts
	address = MPCORE_GIC_CPUIF + ICCICR;
  	*(int *) address = 1;       

	// Configure the Distributor Control Register (ICDDCR) to send pending 
    // interrupts to CPUs 
	address = MPCORE_GIC_DIST + ICDDCR;
  	*(int *) address = 1;          
} //config_GIC

/******************************************************************************
 * config_interrupts: Configure registers in the GIC for an individual 
 *                    interrupt ID.
 *
 * @param	N/A
 * @returns	N/A
 * @note	We configure only the Interrupt Set Enable Registers (ICDISERn) and
 *          Interrupt Processor Target Registers (ICDIPTRn). The default 
 *          (reset) values are used for other registers in the GIC
 *          code is unmodified (see header)
 * @see		N/A
 *****************************************************************************/
void config_interrupt (int N, int CPU_target)
{
	int reg_offset, index, value, address;
    
	/* Configure the Interrupt Set-Enable Registers (ICDISERn). 
	 * reg_offset = (integer_div(N / 32) * 4
	 * value = 1 << (N mod 32) */
	reg_offset = (N >> 3) & 0xFFFFFFFC; 
	index = N & 0x1F;
	value = 0x1 << index;
	address = MPCORE_GIC_DIST + ICDISER + reg_offset;
	// Now that we know the register address and value, 
    // set the appropriate bit 
   *(int *)address |= value;

	/* Configure the Interrupt Processor Targets Register (ICDIPTRn)
	 * reg_offset = integer_div(N / 4) * 4
	 * index = N mod 4 */
	reg_offset = (N & 0xFFFFFFFC);
	index = N & 0x3;
	address = MPCORE_GIC_DIST + ICDIPTR + reg_offset + index;
	// Now that we know the register address and value, 
    // write to (only) the appropriate byte 
	*(char *)address = (char) CPU_target;
} // config_interrupt