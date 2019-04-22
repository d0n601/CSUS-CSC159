// k-type.h, 159

#ifndef __K_TYPE__
#define __K_TYPE__

#include "k-const.h"

typedef void (*func_p_t)(void); // void-return function pointer type
typedef void (*func_p_t2)(int);

typedef enum {UNUSED, READY, RUN, SLEEP, SUSPEND, ZOMBIE, WAIT} state_t;

typedef struct {
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax, entry_id, eip, cs, efl;
} trapframe_t;

typedef struct {
	state_t state;              // A process state: UNUSED, READY, RUN, or SLEEP.
	int wake_centi_sec;
	int run_count;              // A timer interrupt count, reset it when the process is selected.
	int total_count;            // Accumulated timer counts since the creation of the process.
	trapframe_t *trapframe_p;   // Location of the CPU context in the process stack.
	int ppid;					// To record parent PID when the process is created by ForkSR().
	int sigint_handler;			//

} pcb_t;

typedef struct {       			// Generic queue type.
	int size;            		// For a simple queue.
	int q[Q_SIZE];
} q_t;


typedef struct {
	int flag;
	int creater;
	q_t suspend_q;
} mux_t;


typedef struct {
	int tx_missed;   			// When initialized or after output last char.
	int io_base;     			// Terminal port I/O base #.
    int in_mux;                 // To flow-control in_q.
	int out_mux;     			// Flow-control mux.
    q_t in_q;                   // To buffer terminal KB input.
    q_t out_q;       			// Characters to send to terminal buffered here.
    q_t echo_q;                 // To echo back to terminal.
} term_t;


#endif
