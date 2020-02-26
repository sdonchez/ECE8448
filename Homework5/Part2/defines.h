// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 26 February 2020

/******************************************************************************
* defines.h - provides definitions for common values that exist in the system.
*
* File is unchanged with the exception of this header and the inclusion of KEYS
* 2 and 3, original taken from Intel FPGA monitor program interrupt sample 
* program
******************************************************************************/

#define		EDGE_TRIGGERED			0x1
#define		LEVEL_SENSITIVE			0x0
#define		CPU0					0x01	// bit-mask; bit 0 represents cpu0
#define		ENABLE					0x1

#define		KEY0 					1
#define		KEY1 					2
#define		KEY2					4
#define		KEY3					8
#define		NONE					16

#define		USER_MODE				0b10000
#define		FIQ_MODE				0b10001
#define		IRQ_MODE				0b10010
#define		SVC_MODE				0b10011
#define		ABORT_MODE				0b10111
#define		UNDEF_MODE				0b11011
#define		SYS_MODE				0b11111

#define		INT_ENABLE				0b01000000
#define		INT_DISABLE				0b11000000
