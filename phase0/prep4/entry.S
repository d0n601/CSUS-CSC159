#include <spede/machine/asmacros.h>

ENTRY(TimerEntry)
  pusha
  cld
  call CNAME(TimerCode)
  popa
  iret
