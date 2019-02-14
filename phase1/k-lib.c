// k-lib.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"


/* Clear DRAM data block, zero-fill it */
void Bzero(char *p, int bytes) {

   int c;

   for(c = 0; c < Q_SIZE; c++) p[c] = '0'; // Use memset instead???

   for(c = 0; c < Q_SIZE; c++) {
      printf("%c\n", p[c]);
   }

}


/* Return 1 if empty, else 0 */
int QisEmpty(q_t *p) { 
   return p->tail == 0;
}


/* Return 1 if full, else 0 */
int QisFull(q_t *p) { 
   return p->tail == Q_SIZE;
}


/* Dequeue, 1st # in queue; if queue empty, return -1
 * Move rest to front by a notch, set empty spaces -1
*/
int DeQ(q_t *p) { // return -1 if q[] is empty

   int i, x;

   if(QisEmpty(p)) return -1;

   x = p->q[p->head];

   p->size--;

   for(i = 0; i < p->tail-2; i++) {

      if(i < p->tail -1) {
         p->q[i] = p->q[i+1];
      }
      else {
         p->q[i] = NONE;
      }
   }

   return x;
}

/* If not full, enqueue # to tail slot in queue. */
void EnQ(int to_add, q_t *p) {
   if(QisFull(p)) cons_printf("Panic: queue is full, cannot EnQ!\n");
   else {
      p->q[p->size] = to_add;
      p->size++;
      p->tail = (p->tail + 1); //% Q_SIZE;
   }
}