// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use sys-calls

#include "k-const.h"
#include "sys-call.h"  // all service calls used below
#include "k-data.h"



void InitProc(void) {
    int i;
    vid_mux = MuxCreateCall(LOCK);
    while(1) {
        ShowCharCall(0, 0, '.');
        for(i=0; i<LOOP/2; i++) asm("inb $0x80");   // this is also a kernel service

        ShowCharCall(0, 0, ' ');
        for(i=0; i<LOOP/2; i++) asm("inb $0x80");   // this is also a kernel service
    }
}

void UserProc(void) {
    int my_pid = GetPidCall();  // get my PID

    char str1[] = "PID xx is running exclusively using the video display...\0";
    char str2[] = "                                                        \0";

    str1[4] = '0' + my_pid / 10;
    str1[5] = '0' + my_pid % 10;

    while(1) {
        MuxOpCall(vid_mux, LOCK);
        WriteCall(STDOUT, str1);
        SleepCall(50);

        WriteCall(STDOUT, str2);
        SleepCall(50);
        MuxOpCall(vid_mux, UNLOCK);
    }
}