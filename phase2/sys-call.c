// sys-call.c
// calls to kernel services

#include "k-const.h"

int GetPidCall(void) {
    int pid;
    asm("int %1;             // interrupt!
        movl %%eax, %0"     // after, copy eax to variable 'pid'
        : "=g" (pid)         // output
        : "g" (GETPID_CALL)  // input
        : "eax"              // used registers
    );

    return pid;
}

void ShowCharCall(int row, int col, char ch) {
    asm("movl %0, %%eax;     // send in row via eax
        movl %1, %%ebx;            // send in col via ebx
        movl %2, %%ecx;             // send in ch via ecx
        int %3"             // initiate call, %3 gets entry_id
        :                    // no output
        : "g" (row), "g" (col), "g" (ch), "g" (SHOWCHAR_CALL)
        : "eax", "ebx", "ecx"         // affected/used registers
    );
}


void SleepCall(int centi_sec) {  // # of 1/100 of a second to sleep
    asm("movl %0, %%eax;     // send in row via eax
        int %1"
        :
        : "g"(centi_sec), "g"(SLEEP_CALL)
        : "eax"
    );
}


