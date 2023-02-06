#include "../ft_malloc.h"

void    intern_free(void *ptr) {
	if (validate_ptr(ptr - SIZE) == 0) {
		if (m.debug.validate_ptrs == E_OFF) {
			return;
		} else
			abort_validate_ptr(ptr);
	}
	size_t size = GETSIZE(ptr - SIZE);
	if (size < TINYMAXSIZE) {
		desallocate(ptr - SIZE, &m.lst_free_s, size);
	}
	else if (size <= MAXSIZE) {
		desallocate(ptr - SIZE, &m.lst_free_m, size);
	}
	else {
		desallocate_large(ptr - SIZE, size);
	}
}

void    free(void *ptr) {
	pthread_mutex_lock(&mutex_malloc);
	intern_free(ptr);
	pthread_mutex_unlock(&mutex_malloc);
}
