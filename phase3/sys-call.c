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


int MuxCreateCall(int flag){
    int mux_id;
    asm("movl %2, %%eax;
        int %1;
        movl %%eax, %0"
        : "=g"(mux_id)
        : "g"(MUX_CREATE_CALL), "g"(flag)
        : "eax"
    );
    return mux_id;
}


void MuxOpCall(int mux_id, int opcode) {
    asm("movl %0, %%eax;
        movl %1, %%ebx;
        int %2"
        :                       // no output
        : "g" (mux_id), "g"(opcode), "g"(MUX_OP_CALL) // input
        : "eax", "ebx"
    );
}


void WriteCall(int device, char *str) {
    int x = GetPidCall();
    int y = 0;

    if(device ==  STDOUT) {
        while(*str != '\0') {
            ShowCharCall(x, y, *str);
            str++;
            y++;
        }
    }

}




