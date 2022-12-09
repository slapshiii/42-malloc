#include "../ft_malloc.h"

void    ft_free(void *ptr) {
    (void)ptr;
}

void    *ft_malloc(size_t size) {
    static buckets_t b = {
        .lst_page_s = NULL,
        .lst_page_m = NULL,
        .lst_page_l = NULL
    };
    void    *res = NULL;

    if (size < (size_t)getpagesize()/4) {
        res = allocate(&b.lst_page_s, size);
    }
    else if (size < (size_t)getpagesize()) {
        res = allocate(&b.lst_page_m, size);
    }
    else {
        res = allocate_large(&b.lst_page_s, size);
    }
    return (res);
}

void    *ft_realloc(void *ptr, size_t size) {
    (void)ptr;
    (void)size;
    return (NULL);
}
