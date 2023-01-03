
#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>

#include "./libft/libft.h"
#include <stdio.h>

#if defined(__x86_64__)
/* 64 bit detected */
    #define MINSIZE 16UL
    #define SIZE 8UL
    #define MALLOC_ALIGNMENT 8UL
#endif
#if defined(__i386__)
/* 32 bit x86 detected */
    #define MINSIZE 16UL
    #define SIZE 8UL
    #define MALLOC_ALIGNMENT 8UL
#endif

#define ISALLOC(a) (*(size_t*)(a) & 0x01)
#define GETSIZE(a) (*(size_t*)(a) & (~0x07))

/**
 *  small: < pagesize/4
 *  medium: >= pagesize/4 && < pagesize
 *  large: > pagesize
 */
typedef struct  buckets_s {
    void*   lst_page_s;
    void*   lst_page_m;
    void*   lst_page_l;
}               buckets_t;

void    ft_free(void *ptr);
void    *ft_malloc(size_t size);
void    *ft_realloc(void *ptr, size_t size);

void    *allocate(void** l, size_t s);
void    *allocate_large(void** l, size_t s);

void    *format_chunk_a(void *addr, size_t s);
void    *get_first_fit(void* l, size_t s, void **prev_free);
void    hexdump(const void* data, size_t size);

#endif