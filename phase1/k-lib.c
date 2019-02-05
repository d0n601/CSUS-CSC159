// k-lib.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"

// clear DRAM data block, zero-fill it
void Bzero(char *p, int bytes) {
   ...
}

int QisEmpty(q_t *p) { // return 1 if empty, else 0
   ...
}

int QisFull(q_t *p) { // return 1 if full, else 0
   ...
}

// dequeue, 1st # in queue; if queue empty, return -1
// move rest to front by a notch, set empty spaces -1
int DeQ(q_t *p) { // return -1 if q[] is empty
   int ..., ...

   if(QisEmpty(p)) {
      ...
   }

   ...
   ...
   ...
   ...
   ...
}

// if not full, enqueue # to tail slot in queue
void EnQ(int to_add, q_t *p) {
   if(QisFull(p)) {
      cons_printf("Panic: queue is full, cannot EnQ!\n");
      ...
   }

   ...
   ...
}

