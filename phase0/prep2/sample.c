/* sample.c - Sample Main program for SPEDE */

#include <spede/stdio.h>
#include <spede/flames.h>

int main(void) {
    long i;
    i = 128;
    printf("%d Hello world %d \nECS", i, 2*i);
    cons_printf("\n--> Hello world <--\nCPE/CSC\n");
    return 0;
}
