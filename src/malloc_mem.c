#include "../ft_malloc.h"

void	*format_chunk_a(void *addr, size_t s, void *prevfree) {
	size_t	size_allocated;
	size_t	size_free = *(size_t*)addr;
	void	*prev_free = addr + SIZE;
	void	*next_free = addr + SIZE * 2;
	void	*res = addr + SIZE;

	if (s < MINSIZE)
		size_allocated = MINSIZE;
	else if (s % SIZE)
		size_allocated = (s + SIZE) & ~(SIZE - 1);
	else
		size_allocated = s;
	size_t remaining = size_free - size_allocated - 2 * SIZE;
	*(size_t*)(addr) = size_allocated + 1;
	*(size_t*)(res + size_allocated) = size_allocated + 1;
	*(size_t*)(prev_free + SIZE * 2) = (size_t)addr;
	*(size_t*)(res + size_allocated + SIZE) = remaining;
	*(size_t*)(res + size_allocated + 2 * SIZE) = (size_t)next_free;
	*(size_t*)(addr + size_free) = remaining;
	return (res);
}


