
#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stdlib.h>
#include <unistd.h>

/**
 *  Pagesize is 4096
 *  small: < 1024
 *  medium: >= 1024 && < 4096
 *  large: > 4096
 */

typedef struct  buckets_s {
    void* lst_page_s;
    void* lst_page_m;
    void* lst_page_l;
}               buckets_t;

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);

#endif