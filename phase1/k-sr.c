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
      breakpoint();  // cannot continue
   }

   pid = DeQ(&ready_q);                      // alloc PID (1st is 0)

   ...
   ...                                       // clear PCB
   ...                                       // clear stack
   ...                                       // change process state

   if(pid > 0) ...                           // queue to ready_q if > 0

   // point trapframe_p to stack & fill it out
   pcb[run_pid].trapframe_p = ...                             // point to stack top
   pcb[run_pid].trapframe_p--;                                // lower by trapframe size
   pcb[run_pid].trapframe_p->efl = EF_DEFAULT_VALUE|EF_INTR;  // enables intr
   pcb[run_pid].trapframe_p->cs = get_cs() ;                  // dupl from CPU
   pcb[run_pid].trapframe_p->eip = (int)p;                    // set to code
}


/* count run_count and switch if hitting time slice */
void TimerSR(void) {

   outportb(PIC_CONTROL, TIMER_DONE);    // Notify PIC timer done.

   pcb[run_pid].run_count++;             // Count up run_count.
   pcb[run_pid].total_count++;           // Count up total_count.

   if(run_count % TIME_SLICE == 0) {     // If runs long enough.
      EnQ(run_pid, &ready_q);            // Move it to ready_q.
      pcb[run_pid].state = READY;       // Change its state.
      run_pid = NONE;                   // Running proc = NONE.
   }
}

