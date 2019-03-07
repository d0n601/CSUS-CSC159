// k-sr.h, 159

#ifndef __K_SR__
#define __K_SR__

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "k-lib.h"

extern void NewProcSR(func_p_t p);
extern void TimerSR(void);
extern void CheckWakeProc(void);
extern int GetPidSR(void);
extern void ShowCharSR(int, int, char);
extern void SleepSR(int);
extern int MuxCreateSR(int);
extern void MuxOpSR(int, int);

#endif
