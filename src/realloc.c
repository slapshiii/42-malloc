#include "../ft_malloc.h"

void    *intern_realloc(void *ptr, size_t size) {
	if (validate_ptr(ptr - SIZE) == 0) {
		if (m.debug.validate_ptrs == E_OFF) {
			return (NULL);
		} else
			abort_validate_ptr(ptr);
	}
	if (try_extend_chunk(ptr, size)) {
		void *res = intern_malloc(size);
		if (res != NULL) {
			ft_memmove(res, ptr, GETSIZE(ptr - SIZE));
			intern_free(ptr);
			ptr = res;
		}
	}
	return (ptr);
}

void    *realloc(void *ptr, size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	ptr = intern_realloc(ptr, size);
	pthread_mutex_unlock(&mutex_malloc);
	return (ptr);
}
