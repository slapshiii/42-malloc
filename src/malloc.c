#include "../ft_malloc.h"

buckets_t b = {
        .lst_page_s = NULL,
        .lst_page_m = NULL,
        .lst_page_l = NULL,
        .lst_free_s = NULL,
        .lst_free_m = NULL
};

void    ft_free(void *ptr) {
    size_t size = GETSIZE(ptr - SIZE);
    if (size < (size_t)getpagesize()/4) {
       // res = allocate(&b.lst_page_s, &b.lst_free_s, size);
        desallocate(ptr - SIZE, &b.lst_free_s, size);
    }
    else if (size < (size_t)getpagesize()) {
        desallocate(ptr - SIZE, &b.lst_free_m, size);
    }
    else {
        desallocate_large(ptr - SIZE, size);
    }
}

void    *ft_malloc(size_t size) {
    void    *res = NULL;

    if (size < (size_t)getpagesize()/4) {
        res = allocate(&b.lst_page_s, &b.lst_free_s, size);
    }
    else if (size < (size_t)getpagesize()) {
        res = allocate(&b.lst_page_m, &b.lst_free_m, size);
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

void    show_alloc_mem() {
    
}

