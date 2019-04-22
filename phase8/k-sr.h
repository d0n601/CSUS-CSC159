// k-sr.h, 159

#ifndef __K_SR__
#define __K_SR__

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"
#include "tools.h"

extern void NewProcSR(func_p_t p);
extern void TimerSR(void);
extern void CheckWakeProc(void);
extern int GetPidSR(void);
extern void ShowCharSR(int, int, char);
extern void SleepSR(int);
extern int MuxCreateSR(int);
extern void MuxOpSR(int, int);
extern void TermSR(int);
extern void TermTxSR(int);
extern void TermRxSR(int);
extern int ForkSR(void);
extern int WaitSR(void);
extern void ExitSR(int);
extern void ExecSR(int, int);
extern void SignalSR(int, int);
extern void WrapperSR(int, int, int);
#endif
