// k-entry.h
// prototypes those in k-entry.S

#ifndef __K_ENTRY__
#define __K_ENTRY__

#ifndef ASSEMBLER  // skip below if ASSEMBLER defined (from an assembly code)
// since below is not in assembler syntax
__BEGIN_DECLS

#include "k-type.h"               // trapframe_t

extern void TimerEntry(void);            // coded in k-entry.S, assembler won't like this syntax
extern void Loader(trapframe_t *);       // coded in k-entry.S
extern void GetPidEntry(void);
extern void SleepEntry(void);
extern void ShowCharEntry(void);
extern void MuxCreateEntry(void);
extern void MuxOpEntry(void);
extern void Term0Entry(void);
extern void Term1Entry(void);
extern void ForkEntry(void);
extern void WaitEntry(void);
extern void ExitEntry(void);
extern void ExecEntry(void);
extern void SignalEntry(void);
extern void PauseEntry(void);
extern void KillEntry(void);
extern void RandEntry(void);
__END_DECLS

#endif // ifndef ASSEMBLER

#endif // ifndef __K_ENTRY__

