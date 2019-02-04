/* sample.c - Sample Main program for SPEDE */

#include <spede/stdio.h>
#include <spede/flames.h>


void DisplayMsg(int i) {
      printf("%d Hello world %d \nECS", i, 2*i);
      cons_printf("\n--> Hello world <--\nCPE/CSC\n");
}

int main(void) {
    
    int c;
    long i;
    
    i = 111;

    for(c = 0; c < 5; c++) {
         i = i + c;
         DisplayMsg(i); 
    }

    return 0;

}
