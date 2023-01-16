
#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>

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

void    free(void *ptr);
void    *malloc(size_t size);
void    *realloc(void *ptr, size_t size);
void    show_alloc_mem();

void    *allocate(void** l, void **fl, size_t s);
void    *allocate_large(void** l, size_t s);
void	desallocate(void *ptr, void **fl, size_t size);
void	desallocate_large(void *ptr, size_t size);

void    *format_chunk_a(void *addr, void **fl, size_t s);
void	*format_chunk_f(void *addr, void **fl, size_t s);
void	*free_merge_contiguous(void *ptr);
int     try_extend_chunk(void *ptr, size_t size);
void	set_value(void *addr, size_t val);
size_t	get_value(void *addr);

void    *get_first_fit(void* l, size_t s);
void    *get_last_free(void** l);
size_t	print_bucket(void *root);
void    hexdump(const void* data, size_t size);
void	*ft_memmove(void *dst, const void *src, size_t len);

void    ft_putptr_fd(void *p, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
void    ft_puthex_char(unsigned char c, int fd);

#endif