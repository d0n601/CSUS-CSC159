//*********************************************************************
// NAME: Ryan Kozak
//
// entry.S of 159, phase 0, prep 4, Timer Interrupt
//*********************************************************************

#include <spede/machine/asmacros.h>

ENTRY(TimerEntry)
  pusha
  cld
  call CNAME(TimerCode)
  popa
  iret
