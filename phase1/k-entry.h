// k-entry.h
// prototypes those in k-entry.S

#ifndef __K_ENTRY__
#define __K_ENTRY__

#ifndef ASSEMBLER  // skip below if ASSEMBLER defined (from an assembly code)
                   // since below is not in assembler syntax
__BEGIN_DECLS

#include "k-type.h"               // trapframe_t

void TimerEntry(void);            // coded in k-entry.S, assembler won't like this syntax
void Loader(trapframe_t *);       // coded in k-entry.S

__END_DECLS

#endif // ifndef ASSEMBLER

#endif // ifndef __K_ENTRY__

