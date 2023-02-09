#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ft_malloc.h"

#define M 1 //(1024 * 1024)

void print(char *s) {
    write(1, s, strlen(s));
}

int main(int ac, char **av)
{
    /**
     * TEST 0-2
     */
    int i;
    i = 0;
    char *addr;
    while (i < 1024) {
        // addr = (char*)malloc(1024); //test 1
        // addr[0] = 42;               //test 1
        // free(addr);                 //test 2
        ++i;
    }

    /**
     * TEST 3
     */
    // char *addr1;
    // char *addr2;                     //test 3bis
    // char *addr3;
    // addr1 = (char*)malloc(16 * M);
    // strcpy(addr1, "Bonjours\n");
    // print(addr1);
    // addr2 = (char*)malloc(16 * M);   //test 3bis
    // addr3 = realloc(addr1, 128 * M);
    // addr3[127*M] = 42;
    // print(addr3);

    /**
     * TEST 4
     */
    // char *addr1 = malloc(M);
    // free(NULL);
    // free((void*)addr1 + 5);
    // if (realloc((void*)addr1 + 5, 10) == NULL)
    //     print("Bonjours\n");

    /**
     * TEST 5
     */
    // malloc(1024);
    // malloc(1024 * 32);
    // malloc(1024 * 1024);
    // malloc(1024 * 1024 * 16);
    // malloc(1024 * 1024 * 128);
    // show_alloc_mem();

    // char *addr1 = (char*)malloc(16);
    // char *addr2 = (char*)malloc(16);
    // char *addr3 = (char*)malloc(16);
    // char *addr4 = (char*)malloc(16);
    // show_alloc_mem();
    // show_alloc_mem_hex(addr2);
    // free(addr2);
    // show_alloc_mem_hex(addr2);
    // free(addr1);
    // free(addr3);
    // free(addr4);

    return (0);
}