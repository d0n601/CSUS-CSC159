// k-const.h, 159

#ifndef __K_CONST__
#define __K_CONST__

#define NONE -1             // used when none
#define TIMER_INTR 32       // TIMER INTR constant identifier
#define PIC_MASK 0x21       // Programmable Interrupt Controller I/O
#define MASK ~0x01          // mask for Programmable Interrupt Controller
#define PIC_CONTROL 0x20    // Programmable Interrupt Controller I/O
#define TIMER_DONE 0x60     // sent to PIC when timer service done

#define LOOP 1666666        // handly loop limit exec asm("inb $0x80");
#define TIME_SLICE 300      // max timer count, then rotate process
#define PROC_SIZE 20          // max number of processes
#define PROC_STACK_SIZE 4096  // process stack in bytes
#define Q_SIZE 20           // queuing capacity

#define VID_MASK 0x0f00     // foreground bright white, background black
#define VID_HOME (unsigned short *)0xb8000 // home position, upper-left corner


/* Phase 2 constants,  (in intr_table, starting 48 is for kernel services). */
#define GETPID_CALL 48
#define SHOWCHAR_CALL 49
#define SLEEP_CALL 50

/* Phase 3 constants. */
#define MUX_CREATE_CALL 51
#define MUX_OP_CALL 52
#define MUX_SIZE 20
#define STR_SIZE 101
#define LOCK 1
#define UNLOCK 2
#define STDOUT 1

#endif
