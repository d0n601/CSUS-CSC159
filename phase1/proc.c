// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use syscalls

#include "k-const.h"
#include "k-data.h"

/* Slowed down bit more for my VM. */
void Delay(void) {  // delay CPU for half second by 'inb $0x80'
   int i;
   for(i = 0; i < LOOP/4; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond.
}


/* Show ch at row, col. */
void ShowChar(int row, int col, char ch) {
  unsigned short *p = VID_HOME + row*80 + col;   // Upper-left corner of display.
  *p = ch + VID_MASK;
}


/* Initial Process 0. */
void InitProc(void) {
  while(1) {
      ShowChar(0, 0, '.');      // Show a dot at upper left corner on PC.
      Delay();                   // Wait for about half second.
      ShowChar(0, 0, ' ');      // Erase dot.
      Delay();                   // Wait for about half second.
   }
}


/* User Processes. */
void UserProc(void) {
    int i;
    while(1) {
        ShowChar(run_pid, 0, (run_pid/10) + 48 );
        ShowChar(run_pid, 1, (run_pid % 10) + 48);
        Delay();
        ShowChar(run_pid, 0, ' ');
        ShowChar(run_pid, 1, ' ');
        Delay();
   }
}
