#include <spede/machine/asmacros.h>

ENTRY(timer_entry)
  pusha
  cld
  call CNAME(timer_ISR)
  popa
  iret
