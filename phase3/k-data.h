// k-data.h, 159
// kernel data are all declared in main.c during bootstrap
// kernel .c code reference them as 'extern'

#ifndef __K_DATA__
#define __K_DATA__

#include "k-const.h"           // defines PROC_SIZE, PROC_STACK_SIZE
#include "k-type.h"            // defines q_t, pcb_t, ...

extern int run_pid;            // PID of running process
extern q_t pid_q, ready_q, sleep_q;
extern pcb_t pcb[PROC_SIZE];
extern char proc_stack[PROC_SIZE][PROC_STACK_SIZE];
extern struct i386_gate *intr_table;
extern trapframe_t * p;
extern int sys_centi_sec;  // system time in centi-sec, initialize it 0


#endif                         // endif of ifndef
