// main.c, 159
// OS phase 8
//
// Team Name: IDKc (Members: Ryan Kozak, Brad Harris)

#include "k-include.h"  // SPEDE includes
#include "k-entry.h"    // entries to kernel (TimerEntry, etc.)
#include "k-type.h"     // kernel data types
#include "tools.h"      // small handy functions
#include "k-sr.h"       // kernel service routines
#include "proc.h"       // all user process code here

// Kernel data are all here:
int run_pid;                          // current running PID; if -1, none selected
int sys_centi_sec;                    // system time in centi-sec, initialize it 0
q_t pid_q, ready_q, sleep_q, mux_q;   // avail PID and those created/ready to run
pcb_t pcb[PROC_SIZE];               // Process Control Blocks
char proc_stack[PROC_SIZE][PROC_STACK_SIZE];   // process runtime stacks
struct i386_gate *intr_table;    // intr table's DRAM location
int vid_mux;
mux_t mux[MUX_SIZE];     // mutex array
int page_user[PAGE_NUM];
term_t term[TERM_SIZE] = { { TRUE, TERM0_IO_BASE }, { TRUE, TERM1_IO_BASE } };
unsigned rand = 0;


/* Init kernel data */
void InitKernelData(void) {

	int i;
	sys_centi_sec = 0;
	intr_table = get_idt_base();            // Get intr table location.

	/* Clear all queues. */
	Bzero((char *) &pid_q, sizeof(q_t));
	Bzero((char *) &ready_q, sizeof(q_t));
	Bzero((char *) &sleep_q, sizeof(q_t));
	Bzero((char *) &mux_q, sizeof(q_t));

	for (i = 0; i < Q_SIZE; i++) EnQ(i, &pid_q);
	for (i = 0; i < MUX_SIZE; i++) EnQ(i, &mux_q);

	run_pid = NONE;   //set run_pid to NONE
}


/* init kernel control */
void InitKernelControl(void) {
	fill_gate(&intr_table[TIMER_INTR], (int)TimerEntry, get_cs(), ACC_INTR_GATE, 0); // fill out intr table for timer
	fill_gate(&intr_table[GETPID_CALL], (int)GetPidEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[SHOWCHAR_CALL], (int)ShowCharEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[SLEEP_CALL], (int)SleepEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[MUX_CREATE_CALL], (int)MuxCreateEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[MUX_OP_CALL], (int)MuxOpEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[TERM0_INTR], (int)Term0Entry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[TERM1_INTR], (int)Term1Entry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[FORK_CALL], (int)ForkEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[WAIT_CALL], (int)WaitEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[EXIT_CALL], (int)ExitEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[EXEC_CALL], (int)ExecEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[SIGNAL_CALL], (int)SignalEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[PAUSE_CALL], (int)PauseEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[KILL_CALL], (int)KillEntry, get_cs(), ACC_INTR_GATE, 0);
	fill_gate(&intr_table[RAND_CALL], (int)RandEntry, get_cs(), ACC_INTR_GATE, 0);
	outportb(PIC_MASK, MASK);   // mask out PIC for timer
}


/* choose run_pid */
void Scheduler(void) {

	if(run_pid > 0) return; // OK/picked

	if(QisEmpty(&ready_q)) run_pid = 0;

	else {
		pcb[0].state = READY;   // Change state of PID 0 to ready.
		run_pid = DeQ(&ready_q);      // Dequeue ready_q to set run_pid.
	}

	pcb[run_pid].run_count = 0;   // Reset run_count of selected proc.
	pcb[run_pid].state = RUN;    //  Upgrade its state to run.
}


/* OS bootstraps */
int main(void) {

	InitKernelData();       // call to initialize kernel data.
	InitKernelControl();    // call to initialize kernel control.
	NewProcSR(InitProc);    // create InitProc
	Scheduler();            // call Scheduler()

	Loader(pcb[run_pid].trapframe_p);   // call Loader(pcb[run_pid].trapframe_p); load/run it

	return 0; // statement never reached, compiler asks it for syntax
}

/* kernel runs */
void Kernel(trapframe_t *trapframe_p) {

	char ch;

	pcb[run_pid].trapframe_p = trapframe_p; // Save it

	/* Use a "switch(trapframe_p->entry_id)" to call the respective SR, each SR */
	switch(trapframe_p->entry_id) {
		case TIMER_INTR:
			TimerSR();
			break;
		case GETPID_CALL:
			trapframe_p->eax = GetPidSR();
			break;
		case SHOWCHAR_CALL:
			ShowCharSR(trapframe_p->eax, trapframe_p->ebx, trapframe_p->ecx);
			break;
		case SLEEP_CALL:
			SleepSR(trapframe_p->eax);
			break;
		case MUX_CREATE_CALL:
			trapframe_p->eax = MuxCreateSR(trapframe_p->eax);
			break;
		case MUX_OP_CALL:
			MuxOpSR(trapframe_p->eax, trapframe_p->ebx);
			break;
		case TERM0_INTR:
			TermSR(0);
			outportb(PIC_CONTROL, TERM0_DONE_VAL);
			break;
		case TERM1_INTR:
			TermSR(1);
			outportb(PIC_CONTROL, TERM1_DONE_VAL);
			break;
		case FORK_CALL:
			trapframe_p->eax = ForkSR();
			break;
		case WAIT_CALL:
			trapframe_p->eax = WaitSR();
			break;
		case EXIT_CALL:
			ExitSR(trapframe_p->eax);
			break;
		case EXEC_CALL:
			ExecSR(trapframe_p->eax, trapframe_p->ebx);
			break;
		case SIGNAL_CALL:
			SignalSR(trapframe_p->eax, trapframe_p->ebx);
			break;
		case PAUSE_CALL:
			PauseSR();
			break;
		case KILL_CALL:
			KillSR(trapframe_p->eax, trapframe_p->ebx);
			break;
		case RAND_CALL:
			trapframe_p->eax = RandSR();
			break;
		default:
			breakpoint();
	}

	/* keyboard of target PC is pressed */
	if( cons_kbhit() ) {

		ch = cons_getchar();                 //  Read the key.

		if(ch == 'b') breakpoint();             // 'b' for breakpoint.

		else if(ch == 'n') {   // 'n' for new process.
			NewProcSR(UserProc);
			if(rand == 0) rand = sys_centi_sec;
		}
	}

	Scheduler();                             // Call Scheduler()... may need to pick another proc.
	Loader(pcb[run_pid].trapframe_p);        // Loader(...).
}