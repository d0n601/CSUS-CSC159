// tools.c, 159

#include "k-include.h"
#include "k-type.h"
#include "k-data.h"


/* Clear DRAM data block, zero-fill it */
void Bzero(char *p, int bytes) {
        if (bytes > 0) {
                do {
                        *p++ = '\0';
                } while (--bytes);
        }
}

/* Return 1 if empty, else 0 */
int QisEmpty(q_t *p) { return (p->size == 0); }

/* Return 1 if full, else 0 */
int QisFull(q_t *p) { return (p->size == Q_SIZE); }


/* Dequeue, 1st # in queue; if queue empty, return -1
 * Move rest to front by a notch, set empty spaces -1
*/
int DeQ(q_t *p) { // return -1 if q[] is empty

        int ret_val , i;

        if(QisEmpty(p)) return NONE;

        ret_val = p->q[0];

        /* This was what was breaking us - ryan */
        for(i = 0; i < Q_SIZE; i++) {
                if(i < p->size - 1) p->q[i] = p->q[i+1];
                else p->q[i] = -1;
        }

        p->size--;

        return ret_val;
}


/* If not full, enqueue # to tail slot in queue. */
void EnQ(int to_add, q_t *p) {

        if (QisFull(p)) {
                cons_printf("Panic: queue is full, cannot EnQ!\n");
                return;
        }

        if (p->q[p->size] < Q_SIZE) p->q[p->size] = to_add;

        p->size++;
}


/* Copies 'size' of bytes from 'src' to 'dst'. */
void MemCpy(char *dst, char *src, int size) {
        while (size--) *dst++ = *src++;
}



/* Returns TRUE if str1 and str2 are the same, otherwise, 0. */
int StrCmp(char *str1, char *str2) {

        while(TRUE) {

                if(*str1 != *str2) return FALSE;

                if(*str1 == '\0') return TRUE;

                str1++; str2++;
        }
}


/* Converts an unsigned decimal number x < 100000 into str. */
void Itoa(char *s, int x) {

        if(x >= 100000 || x < 0) return;

        // THIS NEEDS TO BE CHANGED, PROBLY FATAL???
        for (; x; x/=10) *--s = '0' + x  % 10;
}
