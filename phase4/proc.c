// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use sys-calls

#include "k-const.h"   // LOOP
#include "sys-call.h"  // all service calls used below
#include "k-data.h"
#include "k-lib.h"      // <------------------------------ NEW!!!
#include "k-include.h"  // <------------------------------ NEW!!!

void InitTerm(int term_no) {  // <------------------------------ NEW!!!
   int i, j;

   Bzero((char *)&term[term_no].out_q, sizeof(q_t));
   term[term_no].out_sem = MuxCreateCall(Q_SIZE);

   outportb(term[term_no].io_base+CFCR, CFCR_DLAB);             // CFCR_DLAB is 0x80
   outportb(term[term_no].io_base+BAUDLO, LOBYTE(115200/9600)); // period of each of 9600 bauds
   outportb(term[term_no].io_base+BAUDHI, HIBYTE(115200/9600));
   outportb(term[term_no].io_base+CFCR, CFCR_PEVEN|CFCR_PENAB|CFCR_7BITS);

   outportb(term[term_no].io_base+IER, 0);
   outportb(term[term_no].io_base+MCR, MCR_DTR|MCR_RTS|MCR_IENABLE);
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");
   outportb(term[term_no].io_base+IER, IER_ERXRDY|IER_ETXRDY); // enable TX & RX intr
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");

   for(j=0; j<25; j++) {                           // clear screen, sort of
      outportb(term[term_no].io_base+DATA, '\n');
      for(i=0; i<LOOP/30; i++)asm("inb $0x80");
      outportb(term[term_no].io_base+DATA, '\r');
      for(i=0; i<LOOP/30; i++)asm("inb $0x80");
   }
/*
   inportb(term_term_no].io_base); // clear key cleared PROCOMM screen
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");
*/
}

void InitProc(void) {
   int i;

   vid_mux = MuxCreateCall(1);  // create/alloc a mutex, flag init 1

   InitTerm(0);  // <------------------------------ NEW!!!
   InitTerm(1);  // <------------------------------ NEW!!!

   while(1) {
      ShowCharCall(0, 0, '.');
      for(i=0; i<LOOP/2; i++) asm("inb $0x80");  // this can also be a kernel service

      ShowCharCall(0, 0, ' ');
      for(i=0; i<LOOP/2; i++) asm("inb $0x80");
   }
}

void UserProc(void) {
   int which_term;
   int my_pid = GetPidCall();  // get my PID

   char str1[STR_SIZE] = "PID    process is running exclusively using the video display...";
   char str2[STR_SIZE] = "                                                                ";

   str1[4] = '0' + my_pid / 10;  // show my PID
   str1[5] = '0' + my_pid % 10;

   which_term = my_pid % 2 == 1? TERM0_INTR : TERM1_INTR; // <---- new

   while(1) {
//      MuxOpCall(vid_mux, LOCK);
      WriteCall(STDOUT, str1);
      WriteCall(which_term, str1);    // <------------------------------ NEW!!!
      WriteCall(which_term, "\n\r");  // <------------------------------ NEW!!!
      SleepCall(50);

      WriteCall(STDOUT, str2);
      SleepCall(50);
//      MuxOpCall(vid_mux, UNLOCK);
   }
}
