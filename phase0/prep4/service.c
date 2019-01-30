/* service.c - Handle timer interrupts  */


#include <spede/flames.h>
#include <spede/machine/io.h>
#include <spede/machine/pic.h>
#include <spede/time.h>

clock_t tick_count = 0; /* Count timer ticks, whatever the rate. */

__BEGIN_DECLS
void timer_ISR(void);
__END_DECLS

void timer_ISR() {
  
  /* Output a character once every second. */
  if( 0 == (++tick_count % CLK_TCK) ) {
    cons_putchar('X');
  } 

  /*
   * Dismiss the timer interrupt. Send a "specific End-Of-Interrupt
   * for IRQ 0" command to the Master Interrupt Control Unit.
   */ 
  outportb( ICU0_IOBASE, SPECIFIC_EOI(IRQ_TIMER));

}
