//*********************************************************************
// NAME: Ryan Kozak
//
// main.c of 159, phase 0, prep 4, Timer Interrupt
//*********************************************************************


#include <spede/flames.h>
#include <spede/machine/io.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/machine/pic.h>
#include <spede/time.h> // From service.c

#define VID_MASK 0x0f00     // foreground bright white, background black.
#define TIMER_INTR 32       // TIMER_INTR constant identifier.



/*Type: Ptr to function take no parameters, return nothing. An ISR */
typedef void (*PFV)(void);

clock_t tick_count = 0; /* Count timer ticks, whatever the rate. (from service.c) */ 

__BEGIN_DECLS
/* first-level handler entry (written in assembly) */
extern void TimerEntry(void);
void TimerCode(void);
__END_DECLS



void TimerCode() {
  /* Output a character once every second. */  
  if( 0 == (++tick_count % CLK_TCK) ) {
    cons_putchar('.');  
  }
  /**
   * Dismiss the timer interrupt. Send a "specific End-Of_interrupt
   * for IRQ 0" command to the Master Interrupt Control Unit.
   */ 
  outportb(ICU0_IOBASE, SPECIFIC_EOI(IRQ_TIMER));
}



int main() {
 
  struct i386_gate *intr_table;

  /* Find out where FLAMES placed the IDT array; */
  intr_table = get_idt_base();

   // set_exception_handler(IRQ_VECTOR(IRQ_TIMER), TimerEntry);
   fill_gate(&intr_table[TIMER_INTR], (int)TimerEntry, get_cs(), ACC_INTR_GATE, 0);


  /**
   * Unmast IRQ 0 (the timer interrupt) while keeping all other IRQs
   * masked (ie, disabled) on the master i8259 PIC (interrupt control 
   * unit zero). Note, a 0 bit = enable.
   */
  outportb(ICU0_IOBASE+1, ~ 0x01); /* ~0x10 = 1111110 (bitwise NOT)  */

  /* We're ready to enable interrupts and start handing the timer.
   * Loop waiting for a keypress on the console, getting interrupted 
   * occasionally.
   */
  EI(); // REMOVE FOR OS CODE!!!
  while(0==cons_kbhit()) {
    IO_DELAY();
  }

  (void)cons_getchar(); /* Eat the keypress.  */

  /* When we exit, FLAMES will disable interrupts for us. */
  return 0;
}
