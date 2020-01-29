//Stephen Donchez
//Dr. Wang
//ECE 8448-DL1
//29 January 2019

/*****************************************************************************************
* Homework 2 Part 2: Iterates through a list of N numbers to determine the largest value
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
			ADD     R3, R4, #8		//Load address 8 greater than is found in R4 (address 
									//of numbers)) into R3
			LDR     R0, [R3]		//Load value at address specified in R3 into R0

LOOP:       SUBS    R2, R2, #1		//Decrement R2 (N) by one, store in R2
			BEQ     DONE			//If R2 = 0, goto done
			ADD     R3, R3, #4		//Add 4 to the value of R3, store in R3
			LDR     R1, [R3]		//Load contents of R3 into R1
			CMP     R0, R1			//Compare the contents of R0 (Previous Number), and R1
									// (Current Number)
			BGE     LOOP			//If value in R0 is >= R1, return to top of loop
			MOV     R0,R1			//otherwise, move the contents of R1 to R0 (such that 
									//R0 always holds the largest known number)
			B       LOOP			//Unconditional return to start of loop

DONE:       STR     R0, [R4]		//Store the contents of R0 at the address specified by
									//R4 (Result)

END:        B       END				//Stay here (so we don't try to execute dataset)

RESULT:     .word   0				//Placeholder value for result of execution
N:          .word   7				//Number of terms to iterate through
NUMBERS:    .word   4, 5, 3, 6		//Values
			.word   1, 8, 2			//Values
			
			.end					//end of program