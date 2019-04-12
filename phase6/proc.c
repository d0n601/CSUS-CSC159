// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use sys-calls

#include "k-const.h"   // LOOP
#include "sys-call.h"  // all service calls used below
#include "k-data.h"
#include "k-lib.h"
#include "k-include.h"
#include "proc.h"


void InitTerm(int term_no) {
   int i, j;

   Bzero((char *)&term[term_no].out_q, sizeof(q_t));
   Bzero((char *)&term[term_no].in_q, sizeof(q_t));      // <------------- new
   Bzero((char *)&term[term_no].echo_q, sizeof(q_t));    // <------------- new
   term[term_no].out_mux = MuxCreateCall(Q_SIZE);
   term[term_no].in_mux = MuxCreateCall(0);              // <------------- new

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
/*  // uncomment this part for VM (Procomm logo needs a key pressed, here reads it off)
   inportb(term[term_no].io_base); // clear key cleared PROCOMM screen
   for(i=0; i<LOOP/2; i++)asm("inb $0x80");
*/
}


void InitProc(void) {
   int i;

   vid_mux = MuxCreateCall(1);  // create/alloc a mutex, flag init 1

   InitTerm(0);
   InitTerm(1);

   while(1) {
      ShowCharCall(0, 0, '.');
      for(i=0; i<LOOP/2; i++) asm("inb $0x80");  // this can also be a kernel service

      ShowCharCall(0, 0, ' ');
      for(i=0; i<LOOP/2; i++) asm("inb $0x80");
   }
}



void UserProc(void) {

   int device, frk, exit_code;
   int my_pid = GetPidCall();

   char str1[STR_SIZE] = "PID    > ";
   char str2[STR_SIZE];
   char str3[STR_SIZE] = "Child PID:   ";
   char str4[STR_SIZE] = "Child Exit Code:   ";

   str1[4] = '0' + my_pid / 10;  // show my PID
   str1[5] = '0' + my_pid % 10;

   device = my_pid % 2 == 1? TERM0_INTR : TERM1_INTR;

   while(TRUE) {

      WriteCall(device, str1);  // prompt for terminal input
      ReadCall(device, str2);   // read terminal input

      // Compare str2 and "fork", if not the same -> "continue;"
      if(StrCmp(str2,"fork") != 0) continue;

      frk = ForkCall();  // Make a ForkCall() and get its return.

      if(frk == NONE) {
         WriteCall(device, "Couldn't Fork!");
         continue;
      }

      if(frk == 0) Aout(device); //child code

      /*
       * Prompt to terminal: the child PID (see demo)  // parent code
       * additional prompt to terminal "\n\r" would look better like the demo
       */
      WriteCall(device, str3); //parent code
      WriteCall(device, "\n\r");

      exit_code = WaitCall();  // Make a WaitCall() and get an exit code from child.

      /*
       *  Prompt to terminal: the child exit code (see demo)
       *  must try your Itoa() (in k-lib.c) to convert exit_code to str for prompting
       *  additional prompt to terminal "\n\r" would look better like the demo.
       */
      Itoa(&str4[16], exit_code);
      WriteCall(device, str4);
      WriteCall(device, "\n\r");
   }

}



void Aout(int device) {

   int i;
   int pid = GetPidCall(); // Get my PID
   char str[STR_SIZE] = "   ( ) Hello, World!\n\r\0";

   str[0] = '0' + pid / 10;
   str[1] = '0' + pid % 10;

   /*
    * In above str, replace xx with my PID, and a alphabet inside
    * the bracket (alphabet B if my PID is 2, C if 3, D if 4, etc.)
    */
   str[4] = 'A' + pid - 1;

   // Prompt to terminal the str.
   WriteCall(device, str); // Use same device as parent.

   // Slide my alphabet across the Target PC display: cycle thru columns 0 to 69.
   for(i = 0; i < 70; i++) {
      ShowCharCall(pid, i, str[4]);
      SleepCall(10);
      ShowCharCall(pid, i, ' ');
   }

   // Call ExitCall with an exit code that is my_pid * 100.
   ExitCall(pid * 100);

}