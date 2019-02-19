// k-lib.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"


/* Clear DRAM data block, zero-fill it */
void Bzero(char *p, int bytes) {
   int c;
   for(c = 0; c < bytes; c++) (int)p[c] = (char *)0;
}


/* Return 1 if empty, else 0 */
int QisEmpty(q_t *p) { return (p->tail == 0); }

/* Return 1 if full, else 0 */
int QisFull(q_t *p) { return (p->tail == Q_SIZE); }


/* Dequeue, 1st # in queue; if queue empty, return -1
 * Move rest to front by a notch, set empty spaces -1
*/
int DeQ(q_t *p) { // return -1 if q[] is empty

   int i, val;

   if(QisEmpty(p)){
      printf("The Queue is empty!");
      return -1;
   }

   val = p->q[0];
   p->tail--;

   for(i=0;i<p->tail-1; i++)  p->q[i] = p->q[i+1];
   for(i=p->tail; i < Q_SIZE; i++) p->q[i] = NONE;

   p->q[p->tail] = -1;

   return val;
}

/* If not full, enqueue # to tail slot in queue. */
void EnQ(int to_add, q_t *p) {
   if(QisFull(p)) {
       cons_printf("Panic: queue is full, cannot EnQ!\n");
       return;
   }

   p->q[p->tail] = to_add;
   p->tail++;
   p->q[p->tail] = NONE;
}