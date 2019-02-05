// k-type.h, 159

#ifndef __K_TYPE__
#define __K_TYPE__

#include "k-const.h"

typedef void (*func_p_t)(void); // void-return function pointer type

typedef enum {UNUSED, READY, RUN} state_t;

typedef struct {
   unsigned int reg[8];
   unsigned int eip;
   unsigned int cs;
   unsigned int efl;
} trapframe_t;

typedef struct {
   ...                       // read in 1.html
   ...
   ...
   ...
} pcb_t;                     

typedef struct {             // generic queue type
  ...                        // for a simple queue
  ...  
} q_t;

#endif
