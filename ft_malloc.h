
#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stdlib.h>
#include <unistd.h>

#include "./libft/libft.h"

#define MINSIZE 16UL
#define SIZE 8UL

/**
 *  small: < pagesize/4
 *  medium: >= pagesize/4 && < pagesize
 *  large: > pagesize
 */
typedef struct  buckets_s {
    t_list*   lst_page_s;
    t_list*   lst_page_m;
    t_list*   lst_page_l;
}               buckets_t;

buckets_t b;

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);

void    *allocate(t_list** l, size_t s);
void    *allocate_large(t_list** l, size_t s);

#endif