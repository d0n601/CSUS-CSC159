// k-sr.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "tools.h"
#include "k-sr.h"
#include "proc.h"


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

	int suspended_pid, sigint_handler, device;
	char ch = inportb(term[term_no].io_base + DATA);  // Read a char from the terminal io_base+DATA.

	if(ch == SIGINT && !QisEmpty(&mux[term[term_no].in_mux].suspend_q)) {

		suspended_pid = mux[term[term_no].in_mux].suspend_q.q[0];
		sigint_handler = pcb[suspended_pid].sigint_handler;

		if(sigint_handler) {
			device = term_no == 0? TERM0_INTR : TERM1_INTR;
			WrapperSR(suspended_pid, sigint_handler, device);
		}
	}

	EnQ(ch, &term[term_no].echo_q);				 // Enqueue char to the terminal echo_q.

	// If char is CR.
	if(ch == '\r') {
		EnQ('\n', &term[term_no].echo_q);  	// Also enqueue NL to the terminal echo_q.
		EnQ('\0', &term[term_no].in_q);		// Enqueue NUL to the terminal in_q.
	}
	else EnQ(ch, &term[term_no].in_q); // Enqueue NUL to the terminal in_q.

	MuxOpSR(term[term_no].in_mux, UNLOCK);   // Unlock the terminal in_mux.

}


int ForkSR(void) {

	int child_pid, d, *p;

	// If pid_q is empty -> 1. prompt a Panic msg to PC, 2. return NONE
	if(QisEmpty(&pid_q)) {
		cons_printf("Panic: No more processes!\n");
		return NONE;
	}

	child_pid = DeQ(&pid_q); // Get a child PID from pid_q.
	Bzero((char *)(&pcb[child_pid]), sizeof(pcb_t)); // Clear the child PCB.

	pcb[child_pid].state = READY;  	// Set the state in the child PCB to READY.
	pcb[child_pid].ppid = run_pid;	// Set the ppid in the child PCB to run_pid.
	EnQ(child_pid, &ready_q); 	    // Enqueue the child PID to ready_q.

	// Get the difference between the locations of the child's stack and the parent's.
	d = &proc_stack[child_pid][0] - &proc_stack[run_pid][0];

	// Copy parent trapframe_p to childs trapframe_p.
	pcb[child_pid].trapframe_p = (trapframe_t *)((int)pcb[run_pid].trapframe_p + d);

	//Copy the parent's proc stack to the child (use your own MemCpy()).
	MemCpy((char *)((int)&(proc_stack[run_pid][0]) + d), &proc_stack[run_pid][0], PROC_STACK_SIZE);

	// Set the eax in the new trapframe to 0 (child proc gets 0 from ForkCall).
	pcb[child_pid].trapframe_p->eax = 0;

	// Apply the location adjustment to esp, ebp, esi, edi in the new trapframe.
	pcb[child_pid].trapframe_p->esp += d;
	pcb[child_pid].trapframe_p->ebp += d;
	pcb[child_pid].trapframe_p->esi += d;
	pcb[child_pid].trapframe_p->edi += d;

	p = (int *)pcb[child_pid].trapframe_p->ebp;   // Set an integer pointer p to ebp in the new trapframe.

	//  While (what p points to is not 0).
	while(*p != 0) {
		*p += d; 		// Apply the location adjustment to the value pointed.
		p = (int *)*p; 		// Set p to the adjusted value (need a typecast).
	}

	return child_pid;    // Return child PID.
}


int WaitSR(void) {

	int i, exit_code;

	// 	Loop thru the PCB array (looking for a ZOMBIE child).
	for(i = 0; i < PROC_SIZE; i++) {
		// The proc ppid is run_pid and the state is ZOMBIE -> break the loop.
		if (pcb[i].ppid == run_pid && pcb[i].state == ZOMBIE) break;
	}

	// If the whole PCB array was looped thru (didn't find any ZOMBIE child)
	if(i == PROC_SIZE) {
		pcb[run_pid].state = WAIT; 	// Alter the state of run_pid to WAIT.
		run_pid = NONE;				// Set run_pid to NONE.
		return NONE;			    // Return NONE.
	}

	exit_code = pcb[i].trapframe_p->eax;  // Get its exit code (from the eax of the child's trapframe).
	pcb[i].state = UNUSED;				  // Alter its state to UNUSED.
	EnQ(i, &pid_q);						  // Enqueue its PID to pid_q.

	return exit_code;  // Return the exit code.
}



void ExitSR(int exit_code) {

	// If the process state of my parent (ppid) is not WAIT.
	if(pcb[pcb[run_pid].ppid].state != WAIT) {
		pcb[run_pid].state = ZOMBIE;    // Alter my state to ZOMBIE!!!
		run_pid = NONE;					// Reset run_pid to NONE;
		return;
	}

	pcb[pcb[run_pid].ppid].state = READY;	// Alter the state of ppid to READY.
	EnQ(pcb[run_pid].ppid, &ready_q);		// Enqueue ppid to .ready_q.
	pcb[pcb[run_pid].ppid].trapframe_p->eax = exit_code;  // pass it the exit code (via eax of parent's trapframe).

	// Reclaim child's resources.
	pcb[run_pid].state = UNUSED;	// Alter its state to UNUSED.
	EnQ(run_pid, &pid_q);			// Enqueue its PID to pid_q.
	run_pid = NONE;					// Reset run_pid to NONE;
}

void ExecSR(int code, int arg) {

	int i, code_page = NONE, stack_page = NONE;
	int * code_addr, * stack_addr;


	for(i = 0; i < PAGE_NUM; i++) {

		if(page_user[i] == NONE) {

			if(code_page == NONE) {
				code_page = i;
				continue;
			}

			// Alwasy true?
			else if(stack_page == NONE) {
				stack_page = i;
			}

			// Alwasy true? code_page always != None??
			if(code_page != NONE && stack_page != NONE) {
				page_user[code_page] = run_pid;
				page_user[stack_page] = run_pid;
				break;
			}

		}

	}

	code_addr = (int*)((code_page * PAGE_SIZE) + RAM);
	stack_addr = (int*)((stack_page * PAGE_SIZE) + RAM);
	MemCpy((char*)code_addr, (char *)code, PAGE_SIZE);
	Bzero((char *)stack_addr, PAGE_SIZE);

	stack_addr = (int*)((int)stack_addr + PAGE_SIZE);
	stack_addr--;
	*stack_addr = arg;
	stack_addr--;

	pcb[run_pid].trapframe_p = (trapframe_t *)stack_addr;
	pcb[run_pid].trapframe_p--;
	pcb[run_pid].trapframe_p->efl = EF_DEFAULT_VALUE | EF_INTR;
	pcb[run_pid].trapframe_p->cs = get_cs();
	pcb[run_pid].trapframe_p->eip = (int)code_addr;

}



void SignalSR(int sig_num, int handler) { pcb[run_pid].sigint_handler = handler; }



void WrapperSR(int pid, int handler, int arg) {

	int *p = (int *)( (int)pcb[pid].trapframe_p + sizeof(trapframe_t) );

	MemCpy((char *)((int)pcb[pid].trapframe_p - sizeof(int[3])), (char *)((int)pcb[pid].trapframe_p), sizeof(trapframe_t));

	pcb[pid].trapframe_p = (trapframe_t *)((int)pcb[pid].trapframe_p - sizeof(int[3]));

	p--;

	*p = arg;

	p--;

	*p = handler;

	p--;

	*p = pcb[pid].trapframe_p->eip;

	pcb[pid].trapframe_p->eip = (int)Wrapper;

}