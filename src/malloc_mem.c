#include "../ft_malloc.h"

int is_allocated(void *addr) {
    return (*(int*)addr & 0x1);
}


void    *format_chunk(void *addr, size_t s) {
    size_t size_allocated;
    size_t size_free = *(size_t*)addr;
    if (s < MINSIZE)
        size_allocated = MINSIZE;
    else if (s % SIZE)
        size_allocated = (s + SIZE) & ~(SIZE - 1);
    else
        size_allocated = s;
    *(int*)addr = size_allocated + 1;
    *(int*)addr + size_allocated
    return (addr + 1);
}