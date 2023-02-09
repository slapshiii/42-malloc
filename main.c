#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//#include "ft_malloc.h"

#define M (1024 * 1024)

void print(char *s) {
    write(1, s, strlen(s));
}

int main(int ac, char **av)
{
    // malloc(1024);
    // malloc(1024 * 32);
    // malloc(1024 * 1024);
    // malloc(1024 * 1024 * 16);
    // malloc(1024 * 1024 * 128);
    // show_alloc_mem();

    // int i;
    // char *addr1 = malloc(M);
    // free(NULL);
    // free((void*)addr1 + 5);
    // if (realloc((void*)addr1 + 5, 10) == NULL)
    //     print("Bonjours\n");

    // char *addr1;
    // char *addr2;
    // char *addr3;

    // addr1 = (char*)malloc(16 * M);
    // strcpy(addr1, "Bonjours\n");
    // print(addr1);
    // addr2 = (char*)malloc(16 * M);
    // addr3 = realloc(addr1, 128 * M);
    // addr3[127*M] = 42;
    // print(addr3);

    int i;
    i = 0;
    char *addr[1024];
    while (i < 1024) {
        addr[i] = (char*)malloc(1024);
        addr[i][0] = 42;
        free(addr[i]);
        ++i;
    }

    // i = 0;
    // while (i < 1024) {
    //     // addr[i] = (char*)malloc(1024);
    //     // addr[i][0] = 42;
    //     free(addr[i]);
    //     ++i;
    // }
    // free(addr[3]);
    // free(addr[2]);
    // free(addr[6]);
    // free(addr[8]);
    // show_alloc_mem();
    // free(addr2);
    // show_alloc_mem();
    // addr[1] = (char*)realloc(addr[1], 2000);
    // show_alloc_mem_hex(addr[1]);
    // free(addr[0]);
    // show_alloc_mem();
    // free(addr1);
    // show_alloc_mem();
    // sleep(5);

    return (0);
}