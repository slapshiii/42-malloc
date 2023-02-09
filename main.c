#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "ft_malloc.h"

#define M (1024 * 1024)

void print(char *s) {
    write(1, s, strlen(s));
}

int main(int ac, char **av)
{
    // ft_malloc(1024);
    // ft_malloc(1024 * 32);
    // ft_malloc(1024 * 1024);
    // ft_malloc(1024 * 1024 * 16);
    // ft_malloc(1024 * 1024 * 128);
    // show_alloc_mem();

    int i;
    char *addr1;
    addr1 = ft_malloc(16);
    ft_free(NULL);
    ft_free((void*)addr1 + 5);
    if (ft_realloc((void*)addr1 + 5, 10) == NULL)
        print("Bonjours\n");

    // char *addr1;
    // char *addr2;
    // char *addr3;

    // addr1 = (char*)ft_malloc(16 * M);
    // strcpy(addr1, "Bonjours\n");
    // print(addr1);
    // addr2 = (char*)ft_malloc(16 * M);
    // addr3 = ft_realloc(addr1, 128 * M);
    // addr3[127*M] = 42;
    // print(addr3);

    // int i;
    // i = 0;
    // char *addr[1024];
    // while (i < 1024) {
    //     addr[i] = (char*)ft_malloc(1024);
    //     addr[i][0] = 42;
    //     ft_free(addr[i]);
    //     ++i;
    // }
    // ft_free(addr[1]);
    // ft_free(addr[3]);
    // ft_free(addr[2]);
    // ft_free(addr[6]);
    // ft_free(addr[8]);
    // show_alloc_mem();
    // ft_free(addr2);
    // show_alloc_mem();
    // addr[1] = (char*)ft_malloc(1040);
    // ft_free(addr[0]);
    // show_alloc_mem();
    // ft_free(addr[1]);
    show_alloc_mem();

    return (0);
}