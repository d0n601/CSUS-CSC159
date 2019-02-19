// main.c, 159
// OS phase 1
//
// Team Name: IDKc (Members: Ryan Kozak, Brad Harris)

#include "k-include.h"  // SPEDE includes
#include "k-entry.h"    // entries to kernel (TimerEntry, etc.)
#include "k-type.h"     // kernel data types
#include "k-lib.h"      // small handy functions
#include "k-sr.h"       // kernel service routines
#include "proc.h"       // all user process code here

// kernel data are all here:
int run_pid;                        // current running PID; if -1, none selected
q_t pid_q, ready_q;                 // avail PID and those created/ready to run
pcb_t pcb[PROC_SIZE];               // Process Control Blocks
char proc_stack[PROC_SIZE][PROC_STACK_SIZE];   // process runtime stacks
struct i386_gate *intr_table;    // intr table's DRAM location


/* init kernel data */
void InitKernelData(void) {

    int i;

    intr_table = get_idt_base();            // get intr table location

    /* clear 2 queues */
    Bzero((char *) &pid_q, sizeof(q_t));
    Bzero((char *) &ready_q, sizeof(q_t));

    for (i = 0; i < Q_SIZE; i++) EnQ(i, &pid_q);

    run_pid = NONE;   //set run_pid to NONE
}


/* init kernel control */
void InitKernelControl(void) {
    fill_gate(&intr_table[TIMER_INTR], (int)TimerEntry, get_cs(), ACC_INTR_GATE, 0); // fill out intr table for timer
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

    TimerSR();                              // Handle timer intr

   /* keyboard of target PC is pressed */
   if( cons_kbhit() ) {
       ch = cons_getchar();                 //  Read the key.
       if(ch=='b') breakpoint();             // 'b' for breakpoint.
       else if(ch == 'n') NewProcSR(UserProc);   // 'n' for new process.
   }

   Scheduler();                             // Call Scheduler()... may need to pick another proc.
   Loader(pcb[run_pid].trapframe_p);        // Loader(...).
}