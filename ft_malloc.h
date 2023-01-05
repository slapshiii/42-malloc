
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

#define ISALLOC(a) (*(size_t*)(a) & 0b001)
#define ISMMAP(a) (*(size_t*)(a) & 0b010)
#define ISMAIN(a) (*(size_t*)(a) & 0b100)

#define GETSIZE(a) (*(size_t*)(a) & (~0b111))

#define BKPTR (2 * SIZE)
#define FDPTR (SIZE)

/**
 *  small: < pagesize/4
 *  medium: >= pagesize/4 && < pagesize
 *  large: > pagesize
 */
typedef struct  buckets_s {
    void*   lst_page_s;
    void*   lst_page_m;
    void*   lst_page_l;
    void*   lst_free_s;
    void*   lst_free_m;
}               buckets_t;

buckets_t b;

void    ft_free(void *ptr);
void    *ft_malloc(size_t size);
void    *ft_realloc(void *ptr, size_t size);
void    show_alloc_mem();

void    *allocate(void** l, void **fl, size_t s);
void    *allocate_large(void** l, size_t s);
void	desallocate(void *ptr, void **fl, size_t size);
void	desallocate_large(void *ptr, size_t size);

void    *format_chunk_a(void *addr, void **fl, size_t s);
void	*format_chunk_f(void *addr, void **fl, size_t s);
void    *get_first_fit(void* l, size_t s);
void	*free_merge_contiguous(void *ptr);

void    *get_last_page_nxt_ptr(void* l);
void    hexdump(const void* data, size_t size);

#endif