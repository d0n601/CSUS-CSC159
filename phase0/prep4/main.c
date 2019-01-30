#include <spede/flames.h>
#include <spede/machine/io.h>
#include <spede/machine/proc_reg.h>
#include <spede/machine/seg.h>
#include <spede/machine/pic.h>


/*Type: Ptr to function take no parameters, return nothing. An ISR */
typedef void (*PFV)(void);


__BEGIN_DECLS
/* first-level handler entry (written in assembly) */
extern void timer_entry(void);
__END_DECLS


/*
 * Ptr to start of Interrupt Descriptor Table (IDT). 
 * It's really a pointer to the start of 256 "i386_gate" structs.
 */
struct i386_gate * idt_table;


/**
 *  Builds an i386 interrupt gate containing the specified "handler"
 *  (IRS entry) address. It stores that gate in the Interrupt Descriptor
 *  Table entry indexed for a specific exception/interrupt.
 */ 
void set_exception_handler(int exception, PFV handler) {

  /* Get address of this particular interrupt descriptor entry: */
  struct i386_gate * gateptr = & idt_table[exception];

  /* Build a valid Interrupt Gate in this IDT entry: */
  fill_gate(gateptr, PTR2INT(handler), get_cs(), ACC_INTR_GATE, 0);
}


/**
 * Get address of IDT then hood the periodic timer IRQ vector. Unmast
 * interrupts. The timer ISR will display ticks while we wait for a 
 * keypress to exit. Incriment screen memory while waiting.
 */
int main() {
  
  volatile uint16 * vidmem = (uint16 *)0X0B8000 + 80*19; /* Text line 20*/

  /* First, drain any stray keypresses: */
  while( cons_kbhit() ) { (void)cons_getchar(); }

  /* Find out where FLAMES placed the IDT array; */
  idt_table = get_idt_base();

  /* Hook the timer interrupt. Set the timer vector (IRQ 0)
   * to point to the assembly entry point for the timer.
   * Now have pointer to existing IDT, fill in timer slot. 
   * The macro IRQ_VECTOR() converts from IRQ# to vector# 
   */
  set_exception_handler(IRQ_VECTOR(IRQ_TIMER), timer_entry);

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
    *vidmem +=1;
  }

  (void)cons_getchar(); /* Eat the keypress.  */

  /* When we exit, FLAMES will disable interrupts for us. */
  return 0;
}
