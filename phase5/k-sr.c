// k-sr.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "k-lib.h"
#include "k-sr.h"


/*
 * To create a process: alloc PID, PCB, and process stack
 * build trapframe, initialize PCB, record PID to ready_q.
 *
 */
void NewProcSR(func_p_t p) {  // arg: where process code starts

	int pid;

	if( QisEmpty(&pid_q) ) {     // May occur if too many been created.
		cons_printf("Panic: no more process!\n");
		return;//breakpoint();  // cannot continue
	}

	pid = DeQ(&pid_q);                                  // alloc PID (1st is 0)

	Bzero((char *)&pcb[pid], sizeof(pcb_t));              // clear PCB
	Bzero((char *)&proc_stack[pid][0], PROC_STACK_SIZE);     // clear stack

	pcb[pid].state = READY;                                 // change process state

	if(pid > 0) EnQ(pid, &ready_q);                       // queue to ready_q if > 0

	// point trapframe_p to stack & fill it out
	pcb[pid].trapframe_p = (trapframe_t *) &proc_stack[pid][PROC_STACK_SIZE - sizeof(trapframe_t)]; // point to stack top
	pcb[pid].trapframe_p->efl = EF_DEFAULT_VALUE|EF_INTR;      // enables intr
	pcb[pid].trapframe_p->cs = get_cs();                      // dupl from CPU
	pcb[pid].trapframe_p->eip = (unsigned int)p;              // set to code
}


/* New subroutine to add to k-sr.c (preferrably before TimerSR): */
void CheckWakeProc(void) {

	int i,l, p_id;

	l = sleep_q.size;   // See how many proc are there in sleep_q.

	/* loop for that many times */
	for(i = 0; i < l; i++) {

		p_id = DeQ(&sleep_q);    // Dequeue sleep_q to get PID.

		/* Using the PID to check if its wake time is up? */
		if(pcb[p_id].wake_centi_sec <= sys_centi_sec) {
			pcb[p_id].state = READY;        // Alter PID's proc state.
			EnQ(p_id, &ready_q);            // Add PID to ready_q.
		}
		else {
			EnQ(p_id, &sleep_q);  // No: enqueue PID back to sleep_q.
		}
	}
}


/* Count run_count and switch if hitting time slice */
void TimerSR(void) {

	outportb(PIC_CONTROL, TIMER_DONE);    // Notify PIC timer done.

	sys_centi_sec++;                      // Upcount sys_centi_sec.

	CheckWakeProc();                      // Call a new subroutine.

	if(!run_pid) return;                  // If run_pid is 0, just return here

	pcb[run_pid].run_count++;             // Count up run_count.
	pcb[run_pid].total_count++;           // Count up total_count.

	if(pcb[run_pid].run_count == TIME_SLICE) {     // If runs long enough.
		EnQ(run_pid, &ready_q);            // Move it to ready_q.
		pcb[run_pid].state = READY;       // Change its state.
		run_pid = NONE;                   // Running proc = NONE.
	}
}


/* Return id of running process. */
int GetPidSR(void) { return run_pid; }


/* Show ch at row, col. */
void ShowCharSR(int row, int col, char ch) {
	unsigned short *p = VID_HOME + row*80 + col;   // Upper-left corner of display.
	*p = ch + VID_MASK;
}


void SleepSR(int centi_sec) {
	pcb[run_pid].wake_centi_sec = sys_centi_sec + centi_sec;
	pcb[run_pid].state = SLEEP;
	EnQ(run_pid, &sleep_q);
	run_pid = NONE;
}



int MuxCreateSR(int flag) {

	int mux_id;

	mux_id = DeQ(&mux_q);
	Bzero((char *)&mux[mux_id].suspend_q, sizeof(q_t));
	mux[mux_id].flag = flag;
	mux[mux_id].creater = run_pid;

	return mux_id;
}



void MuxOpSR(int mux_id, int opcode) {

	if(opcode == LOCK) {

		if(mux[mux_id].flag > 0) mux[mux_id].flag--;

		else {
			EnQ(run_pid, &mux[mux_id].suspend_q);
			pcb[run_pid].state = SUSPEND;
			run_pid = NONE;
		}

	}
	else if(opcode == UNLOCK) {

		if(QisEmpty(&mux[mux_id].suspend_q)) mux[mux_id].flag++;

		else {
			int proc_id;
			proc_id = DeQ(&mux[mux_id].suspend_q);
			EnQ(proc_id, &ready_q);
			pcb[proc_id].state = READY;
		}
	}
}



void TermSR(int term_no) {

	int type = inportb(term[term_no].io_base + IIR);

	if(type == TXRDY) TermTxSR(term_no);

	else if(type == RXRDY) TermRxSR(term_no);

	if(term[term_no].tx_missed) TermTxSR(term_no);
}



void TermTxSR(int term_no) {

    char ch;

	//  If both echo_q and out_q are empty.
	if( QisEmpty(&term[term_no].out_q) && QisEmpty(&term[term_no].echo_q) ) {
		term[term_no].tx_missed = TRUE; // Set tx_missed flag to TRUE.
		return;
	}

	// Echo_q is not empty.
	if(!QisEmpty(&term[term_no].echo_q)) {
		ch = DeQ(&term[term_no].echo_q);   // Get a char from there.
	}

	else {
		ch = DeQ(&term[term_no].out_q);          // Get char from out_q.
        MuxOpSR(term[term_no].out_mux, UNLOCK);  // Unlock out_mux.

    }
	outportb(term[term_no].io_base + DATA, ch);  // Send char via outportb.
	term[term_no].tx_missed = FALSE;			 // Set tx_missed to FALSE.
}


void TermRxSR(int term_no) {

	char ch = inportb(term[term_no].io_base + DATA);  // Read a char from the terminal io_base+DATA.
	EnQ(ch, &term[term_no].echo_q);				 // Enqueue char to the terminal echo_q.

	// If char is CR.
	if(ch == '\r') {
		EnQ('\n', &term[term_no].echo_q);  	// Also enqueue NL to the terminal echo_q.
		EnQ('\0', &term[term_no].in_q);		// Enqueue NUL to the terminal in_q.
	}
	else EnQ(ch, &term[term_no].in_q); // Enqueue NUL to the terminal in_q.

	MuxOpSR(term[term_no].in_mux, UNLOCK);   // Unlock the terminal in_mux.
}