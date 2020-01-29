//Stephen Donchez
//Dr. Wang
//ECE 8448-DL1
//29 January 2019

/*****************************************************************************************
* Homework 2 Part 3: Iterates through a list of N numbers to determine the largest value
*					 Contains modifications from part 2 to utilize a subroutine instead of
*					 branching.
*
* @param N: Number of elements to iterate through
* @param NUMBERS: Values to be iterated through, each occupying 32 bits of memory
* @returns RESULT: Largest value from the list of inputs, occupying 32 bits of memory
* @note: specific function of each executable line of code follows
*****************************************************************************************/

			.text					//executable code follows
			.global _start			//start of program
_start:
			LDR     R4, =RESULT		//Load Result into Reg 4
			LDR     R2, [R4, #4]	//Load value of location with address 4 greater than 
									//is found in R4 (N) into R2
			ADD     R1, R4, #8		//Load address 8 greater than is found in R4 (address 
									//of numbers)) into R1
			BL 		SUB1			//call the SUB1 subroutine
			STR     R0, [R4]		//Store the contents of R0 at the address specified by
									//R4 (Result)
END:        B       END				//Stay here (so we don't try to execute dataset or
									//subroutine)

SUB1:       LDR		R0, [R1]		//Load contents of address in R1 (1st number) into R0
LOOP:		SUBS    R2, R2, #1		//Decrement R2 (N) by one, store in R2
			BEQ     DONE			//If R2 = 0, goto done
			ADD     R1, R1, #4		//Add 4 to the value of R1, store in R1
			LDR		R3, [R1]		//Load contents of address in R1 into R3
			CMP     R0, R3			//Compare the contents of R0 (Previous Value), and R3
									// (Current Number)
			BGE     LOOP			//If value in R0 is >= R3, return to top of loop
			MOV     R0,R3			//otherwise, move the contents of R0 to R2 (such that 
									//R2 always holds the largest known number)
			B       LOOP			//Unconditional return to start of loop

DONE:		BX		lr				//return from subroutine



RESULT:     .word   0				//Placeholder value for result of execution
N:          .word   7				//Number of terms to iterate through
NUMBERS:    .word   4, 5, 3, 6		//Values
			.word   1, 8, 2			//Values
			
			.end					//end of program