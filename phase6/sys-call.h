#ifndef _SYS_CALL_
#define _SYS_CALL_

extern int GetPidCall(void);
extern void ShowCharCall(int, int, char);
extern void SleepCall(int);
extern int MuxCreateCall(int);
extern void MuxOpCall(int, int);
extern void WriteCall(int, char *);
extern void ReadCall(int, char *);
extern int ForkCall(void);
extern int WaitCall(void);
extern void ExitCall(int);
#endif //_SYS_CALL_
