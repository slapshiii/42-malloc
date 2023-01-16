#include "../ft_malloc.h"

buckets_t b = {
        .lst_page_s = NULL,
        .lst_page_m = NULL,
        .lst_page_l = NULL,
        .lst_free_s = NULL,
        .lst_free_m = NULL
};

void    free(void *ptr) {
    size_t size = GETSIZE(ptr - SIZE);
    if (size < (size_t)getpagesize()/4) {
        desallocate(ptr - SIZE, &b.lst_free_s, size);
    }
    else if (size < (size_t)getpagesize()) {
        desallocate(ptr - SIZE, &b.lst_free_m, size);
    }
    else {
        desallocate_large(ptr - SIZE, size);
    }
}

void    *malloc(size_t size) {
    void    *res = NULL;

    if (size < (size_t)getpagesize()/4) {
        res = allocate(&b.lst_page_s, &b.lst_free_s, size);
    }
    else if (size < (size_t)getpagesize()) {
        res = allocate(&b.lst_page_m, &b.lst_free_m, size);
    }
    else {
        res = allocate_large(&b.lst_page_l, size);
    }
    return (res);
}

void    *realloc(void *ptr, size_t size) {
    if (try_extend_chunk(ptr, size)) {
        void *res = malloc(size);
        if (res != NULL) {
            size = (GETSIZE(ptr - SIZE) < size) ? GETSIZE(ptr - SIZE) : size;
            ft_memmove(res, ptr, size);
            free(ptr);
        }
        return (res);
    }
    return (ptr);
}

void    show_alloc_mem() {
    size_t total = 0;
    ft_putstr_fd("TINY : ", 1);
	ft_putptr_fd(b.lst_page_s, 1);
    ft_putstr_fd("\n", 1);
    total += print_bucket(b.lst_page_s);
    ft_putstr_fd("SMALL : ", 1);
	ft_putptr_fd(b.lst_page_m, 1);
    ft_putstr_fd("\n", 1);
    total += print_bucket(b.lst_page_m);
    ft_putstr_fd("LARGE : ", 1);
	ft_putptr_fd(b.lst_page_l, 1);
    ft_putstr_fd("\n", 1);
    total += print_bucket(b.lst_page_l);
    ft_putstr_fd("Total : ", 1);
	ft_putnbr_fd(total, 1);
    ft_putstr_fd(" bytes\n", 1);
}

