// k-sr.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "k-lib.h"
#include "k-sr.h"

// to create a process: alloc PID, PCB, and process stack
// build trapframe, initialize PCB, record PID to ready_q
void NewProcSR(func_p_t p) {  // arg: where process code starts
   int pid;

   if( pid_q is empty ) {     // may occur if too many been created
      cons_printf("Panic: no more process!\n");
      ...                     // cannot continue, alternative: breakpoint();
   }

   ...                                       // alloc PID (1st is 0)
   ...                                       // clear PCB
   ...                                       // clear stack
   ...                                       // change process state

   if(pid > 0) ...                           // queue to ready_q if > 0

// point trapframe_p to stack & fill it out
   pcb[...].trapframe_p = ...                // point to stack top
   pcb[...].trapframe_p--;                   // lower by trapframe size
   pcb[...].trapframe_p->efl = EF_DEFAULT_VALUE|EF_INTR; // enables intr
   pcb[...].trapframe_p->cs = get_cs();                  // dupl from CPU
   pcb[...].trapframe_p->eip =                           // set to code
}

// count run_count and switch if hitting time slice
void TimerSR(void) {
   outportb(...                              // notify PIC timer done

   ...                                       // count up run_count
   ...                                       // count up total_count

   if(...                          ) {       // if runs long enough
      ...                                    // move it to ready_q
      ...                                    // change its state
      ...                                    // running proc = NONE
   }
}

