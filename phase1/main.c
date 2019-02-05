// main.c, 159
// OS phase 1
//
// Team Name: ??????? (Members: ??????...)

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

void InitKernelData(void) {         // init kernel data
   int i;
      
   ... = get_idt_base();            // get intr table location

   Bzero(...);                      // clear 2 queues
   Bzero(...);
   for(i=...                        // put all PID's to pid queue

   set run_pid to NONE

void InitKernelControl(void) {      // init kernel control
   fill_gate(...);                  // fill out intr table for timer
   outportb(...);                   // mask out PIC for timer
}

void Scheduler(void) {      // choose run_pid
   if run_pid is greater than 0, just return; // OK/picked

   if ready_q is empty:
      pick 0 as run_pid     // pick InitProc
   else:
      change state of PID 0 to ready
      dequeue ready_q to set run_pid

   ... ;                    // reset run_count of selected proc
   ... ;                    // upgrade its state to run
}

int main(void) {                          // OS bootstraps
   call to initialize kernel data
   call to initialize kernel control

   call NewProcSR(InitProc) to create it  // create InitProc
   call Scheduler()
   call Loader(pcb[run_pid].trapframe_p); // load/run it

   return 0; // statement never reached, compiler asks it for syntax
}

void Kernel(trapframe_t *trapframe_p) {           // kernel runs
   char ch;

   pcb[...].trapframe_p = trapframe_p; // save it

   call TimerSR();                     // handle timer intr

   if KB of PC is pressed {            // check if keyboard pressed
      read the key
      if it's 'b':                     // 'b' for breakpoint
         ...                           // let's go to GDB
         break;
      if it's 'n':                     // 'n' for new process
         call NewProcSR(UserProc);     // create a UserProc
     }
   }
   call Scheduler()    // may need to pick another proc
   call Loader(...)
}

