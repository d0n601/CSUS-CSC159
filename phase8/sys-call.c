// sys-call.c
// calls to kernel services

#include "k-const.h"
#include "k-data.h"
#include "tools.h"


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

        term_no = device == TERM0_INTR ? 0 : 1;  // Determine which term_no.

		while(*str != '\0') {

			MuxOpCall(term[term_no].out_mux, LOCK);

			EnQ(*str, &term[term_no].out_q);

			/* asm("int %0": :"g" (device)); */
			if(device == TERM0_INTR) asm("int %0": : "g" (TERM0_INTR));
			else asm("int %0": :"g" (TERM1_INTR));

			str++;
		}
	}
}



void ReadCall(int device, char * str) {

    char ch;
    int term_no, ct = 0; // Number of chars gathered so far = 0

    term_no = device == TERM0_INTR ? 0 : 1;  // Determine which term_no.

    while(TRUE) {

        MuxOpCall(term[term_no].in_mux, LOCK);  // Lock the in_mux of the terminal.

        ch = DeQ(&term[term_no].in_q); // Dequeue a char from out_q of the terminal.

        *str = ch; // Set where the str points to to char.

        if(ch == '\0') return;  // If char is NUL -> return.

        ct++; str++;  // Advance both str pointer and char count.

        // If char count is at the last available space of the given string.
        if(ct == STR_SIZE - 1) {
            *str = '\0';  // Set where str points to to NUL.
            return;
        }

    }
}


/*
 *	Returns the PID of the new child process to the calling
 *	(parent) process, or NONE if the call fails. If succeeded,
 *	the child gets 0.
 */
int ForkCall(void) {

	int exit_code;

	asm("
		int %1;
		movl %%eax, %0"
		: "=g" (exit_code)
		: "g" (FORK_CALL)
		: "eax"
	);

	return exit_code;
}


/* Returns the exit code from the child process (as it exits) */
int WaitCall(void) {

	int exit_code;

	asm("
		int %1;
		movl %%eax, %0"
		: "=g" (exit_code)
		: "g" (WAIT_CALL)
		: "eax"
	);

	return exit_code;
}


/* Called by an exiting process, an exit code is given to return to its parent process
*/
void ExitCall(int exit_code) {
	asm("
		movl %0, %%eax;
		int %1"
		:
		: "g" (exit_code), "g" (EXIT_CALL)
		: "eax"
	);
}


void ExecCall(int code, int arg) {
	asm("
		movl %0, %%eax;
		movl %1, %%ebx;
		int %2"
		:
		: "g"(code), "g" (arg), "g" (EXEC_CALL)
		: "eax", "ebx"
	);
}


void SignalCall(int sig_num, int handler) {
	asm("
		movl %0, %%eax;
		movl %1, %%ebx;
		int %2"
		:
		: "g" (sig_num), "g" (handler), "g" (SIGNAL_CALL)
		: "eax", "ebx"
	);
}