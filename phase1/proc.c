// proc.c, 159
// all user processes are coded here
// processes do not R/W kernel data or code, must use syscalls

#include "k-const.h"
#include "k-data.h"


void Delay(void) {  // delay CPU for half second by 'inb $0x80'
   int i;
   for(i = 0; i < LOOP/2; i++) asm("inb $0x80"); // each inb delays CPU .6 microsecond
}


/* Show ch at row, col. */
void ShowChar(int row, int col, char ch) {
  unsigned short *p = VID_HOME + row + col;   // Upper-left corner of display.
  *p = ch + VID_MASK;
  outportb(PIC_CONTROL, TIMER_DONE);
}


void InitProc(void) {

  while(1) {
      printf(".");//ShowChar(0, 0, '.');      // Show a dot at upper left corner on PC.
      Delay();                   // Wait for about half second.
      printf("X");//ShowChar(0, 0, ' ');      // Erase dot.
      Delay();                   // Wait for about half second.
   }

}


void UserProc(void) {
   while(1) {
      ShowChar(0, 0, (char)run_pid);       // Show 1st digit of my PID at row run_pid+1, 1st col // *vidmem +=1;
      ShowChar(0, 0, (char)run_pid);       // Show 2nd digit of my PID at row run_pid+1, 2nd col // *vidmem +=1;
      Delay();          // Wait for about half second.
      // ????/ ShowChar();       // Erase above writing (one digit at a time).
      Delay();          // Wait for about half second.
   }
}
