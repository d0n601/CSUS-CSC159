// k-const.h, 159

#ifndef __K_CONST__
#define __K_CONST__

#define NONE -1             // used when none
#define TIMER_INTR 32       // TIMER INTR constant identifier
#define PIC_MASK 0x21       // Programmable Interrupt Controller I/O
#define MASK 0xffffffe6     // new mask 1111_1111_1111_1111_1111_1111_1110_0110
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

/* Phase 4 constants. */
#define TERM_SIZE 2         // p4, 2 terminals
#define TERM0_INTR 35       // 1st terminal intr ID
#define TERM1_INTR 36       // 2nd terminal intr ID
#define TERM0_IO_BASE 0x2f8 // I/O base of 1st term
#define TERM1_IO_BASE 0x3e8 // I/O base of 2nd term
#define TERM0_DONE_VAL 0x63 // to send to PIC when 1st term served
#define TERM1_DONE_VAL 0x64 // to send to PIC when 1st term served
#define TXRDY 2             // terminal has a Transmit Ready event
#define RXRDY 4             // terminal has a Receive Ready event
#define TRUE 1
#define FALSE 0

/* Phase 5 constants. */
#define FORK_CALL 53
#define WAIT_CALL 54
#define EXIT_CALL 55


#endif
