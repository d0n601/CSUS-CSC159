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

