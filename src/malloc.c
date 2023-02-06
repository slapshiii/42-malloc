#include "../ft_malloc.h"

void	*intern_malloc(size_t size) {
	void    *res = NULL;
	if (size < MINSIZE)
		size = MINSIZE;
	else if (size % SIZE)
		size = (size + SIZE) & ~(SIZE - 1);
	if (size < TINYMAXSIZE) {
		res = allocate(&m.lst_page_s, &m.lst_free_s, size);
	}
	else if (size <= MAXSIZE) {
		res = allocate(&m.lst_page_m, &m.lst_free_m, size);
	}
	else {
		res = allocate_large(&m.lst_page_l, size);
	}
	return res;
}

void    *malloc(size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	void    *res = NULL;
	res = intern_malloc(size);
	pthread_mutex_unlock(&mutex_malloc);
	return (res);
}
