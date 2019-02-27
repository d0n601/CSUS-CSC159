// k-include.h, CpE/CSc159
// SPEDE library headers, ~spede2/Target-i386/Common/include/spede

#ifndef __K_INCLUDE__
#define __K_INCLUDE__

#include <spede/stdio.h> // printf(), sprintf(), vprintf(), sscanf(), etc.
#include <spede/flames.h> // IO_DELAY(), breakpoint(), cons_putchar(), cons_getchar(), cons_kbhit(),
#include <spede/sys/cdefs.h> // __BEGIN_DECLS, __END_DECLS, etc.
#include <spede/machine/io.h> // inportb(), outportb(), inportw(), etc.
#include <spede/machine/proc_reg.h> // get_idt_base(), get_cs(), set_cr3(), etc
#include <spede/machine/seg.h> // struct i386_gate, fill_gate(), etc.
#include <spede/machine/asmacros.h> // ASM(), CNAME(), ENTRY(), etc.

//#include <spede/machine/rs232.h>    // for terminals, IRQ3 constants

#endif

/*  I/O port 0x80 is reserved and will never be used.  If you
 *  access this port, you create a delay of three cycles at 8.33MHz,
 *  plus the I/O access, which takes two clock cycles (total of 5).
 *  An unmapped I/O port goes out to the ISA bus, which runs with
 *  a 8.33MHz clock. Especially true in PCI systems.
 *  Two of these in a row give a 1.2 microsecond delay.
 *       Attributed to James van Artsdalen.
 */
// #define  IO_DELAY()  ASM ("inb $0x80,%%al" : : : "eax")
