// k-type.h, 159

#ifndef __K_TYPE__
#define __K_TYPE__

#include "k-const.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {UNUSED, READY, RUN, SLEEP, SUSPEND} state_t;

typedef struct {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax, entry_id, eip, cs, efl;
} trapframe_t;

typedef struct {
    state_t state;              // a process state: UNUSED, READY, RUN, or SLEEP.
    int wake_centi_sec;
    int run_count;              // a timer interrupt count, reset it when the process is selected.
    int total_count;            // accumulated timer counts since the creation of the process.
    trapframe_t *trapframe_p;   // location of the CPU context in the process stack.
} pcb_t;

typedef struct {       // generic queue type
  int size;            // for a simple queue
  int q[Q_SIZE];
} q_t;


typedef struct {
    int flag;
    int creater;
    q_t suspend_q;
} mux_t;


#endif
