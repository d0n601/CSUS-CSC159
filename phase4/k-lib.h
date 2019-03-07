// k-lib.h, 159


#ifndef __K_LIB__
#define __K_LIB__

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"

extern void Bzero(char *p, int bytes);
extern int QisEmpty(q_t *p);
extern int QisFull(q_t *p);
extern int DeQ(q_t *p);
extern void EnQ(int to_add, q_t *p);

#endif
