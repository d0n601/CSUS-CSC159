// sys-call.c
// calls to kernel services

#include "k-const.h"
#include "k-data.h"
#include "k-lib.h"

int GetPidCall(void) {
	int pid;
	asm("
		int %1;             // interrupt!
		movl %%eax, %0"     // after, copy eax to variable 'pid'
		: "=g" (pid)         // output
		: "g" (GETPID_CALL)  // input
		: "eax"              // used registers
	);

	return pid;
}

void ShowCharCall(int row, int col, char ch) {
	asm("
		movl %0, %%eax;     // send in row via eax
		movl %1, %%ebx;            // send in col via ebx
		movl %2, %%ecx;             // send in ch via ecx
		int %3"             // initiate call, %3 gets entry_id
		:                    // no output
		: "g" (row), "g" (col), "g" (ch), "g" (SHOWCHAR_CALL)
		: "eax", "ebx", "ecx"         // affected/used registers
	);
}


void SleepCall(int centi_sec) {  // # of 1/100 of a second to sleep
	asm("
		movl %0, %%eax;     // send in row via eax
		int %1"
		:
		: "g"(centi_sec), "g"(SLEEP_CALL)
		: "eax"
	);
}


int MuxCreateCall(int flag){
	int mux_id;
	asm("
		movl %2, %%eax;
		int %1;
		movl %%eax, %0"
		: "=g"(mux_id)
		: "g"(MUX_CREATE_CALL), "g"(flag)
		: "eax"
	);
	return mux_id;
}


void MuxOpCall(int mux_id, int opcode) {
	asm("
		movl %0, %%eax;
		movl %1, %%ebx;
		int %2"
		:                       // no output
		: "g" (mux_id), "g"(opcode), "g"(MUX_OP_CALL) // input
		: "eax", "ebx"
	);
}


void WriteCall(int device, char *str) {
	
	int x = GetPidCall();
	int y = 0;
	int term_no;

	if(device ==  STDOUT) {
		while(*str != '\0') {
			ShowCharCall(x, y, *str);
			str++;
			y++;
		}
	}
	else {

		if(device == TERM0_INTR) term_no = 0;

		else term_no = 1;

		while(*str != '\0') {

			MuxOpCall(term[term_no].out_sem, LOCK);

			EnQ((int)str, &term[term_no].out_q);

			if(device == TERM0_INTR) asm("int %0":: "g" (TERM0_INTR));

			else asm("int %0":: "g" (TERM1_INTR));

			str++;
		}
	}
}



