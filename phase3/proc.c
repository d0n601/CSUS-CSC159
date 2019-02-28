// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use sys-calls

#include "k-const.h"   // LOOP
#include "sys-call.h"  // all service calls used below

void InitProc(void) {
    int i;
    while(1) {
        ShowCharCall(0, 0, '.'); // THIS PROBLY NEEDS TO BE MuxOpCall(vid_mux, LOCK);
        for(i=0; i<LOOP/2; i++) asm("inb $0x80");   // this is also a kernel service

        ShowCharCall(0, 0, ' '); // THIS PROBLY NEEDS TO BE MuxOpCall(vid_mux, LOCK);
        for(i=0; i<LOOP/2; i++) asm("inb $0x80");   // this is also a kernel service
    }
}

void UserProc(void) {
    int my_pid = GetPidCall();  // get my PID

    char str1[] = "PID xx is running, no body else is using video?";
    char str2[] = "                                               ";

    str1[4] = '0' + my_pid / 10;
    str1[5] = '0' + my_pid % 10;

    while(1) {
        MuxOpCall(vid_mux, LOCK);
        WriteCall(STDOUT, str1);
        SleepCall(500);
        WriteCall(STDOUT, str1);
        SleepCall(500);
        MuxOpCall(vid_mux, UNLOCK);
    }
}