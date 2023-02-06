#include "../ft_malloc.h"

void    *get_first_fit(void* l, size_t s) {
	void *c = l;
	while (c != NULL && GETSIZE(c) < s) {
		c = (void*)get_value(c + FDPTR);
	}
	return (c);
}

void    *get_last_free(void** l) {
	void *c = *l;
	while (get_value(c + FDPTR)) {
		c = (void*)get_value(c + FDPTR);
	}
	return (c);
}

size_t	get_max_zone(size_t size) {
	if (size < TINYMAXSIZE)
		return (SMALLZONE);
	return (MEDIUMZONE);
}

void	set_value(void *addr, size_t val) {
	*(size_t*)(addr) = val;
}

size_t	get_value(void *addr) {
	return *(size_t*)(addr);
}
