#include "../ft_malloc.h"

/**
 *     chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Size of previous chunk, if unallocated (P clear)  |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     `head:' |             Size of chunk, in bytes                     |A|0|P|
 *       mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Back pointer to previous chunk in freelist        |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Forward pointer to next chunk in freelist         |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Unused space (may be 0 bytes long)                .
 *             .                                                               .
 *             .                                                               |
 * nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     `foot:' |             Size of chunk, in bytes                           |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Size of next chunk, in bytes                |A|0|0|
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

void	*format_chunk_a(void *addr, void **fl, size_t s) {
	size_t	prev_free = *(size_t*)(addr + SIZE);
	size_t	next_free = *(size_t*)(addr + SIZE * 2);
	void	*res = (void *)(addr + SIZE);

	size_t	size_free = GETSIZE(addr);
	size_t	size_allocated;
	if (s < MINSIZE)
		size_allocated = MINSIZE;
	else if (s % SIZE)
		size_allocated = (s + SIZE) & ~(SIZE - 1);
	else
		size_allocated = s;
	size_t remaining = size_free - size_allocated - 2 * SIZE;
	if (remaining < 2 * SIZE) {
		printf("IS ENOUGHT?\n%ld size free\n%ld size alloc\n%ld remain\n", size_free, size_allocated, remaining);
		size_allocated = size_free;
	}
	*(size_t*)(addr) = size_allocated + 1;
	*(size_t*)(res + size_allocated) = size_allocated + 1;
	if ((size_t*)prev_free != NULL){
		*(size_t*)(prev_free + SIZE * 2) = (remaining < 4 * SIZE) ? next_free : (size_t)res + size_allocated + SIZE;
	}
	if ((size_t*)next_free != NULL) {
		*(size_t*)(next_free + SIZE) = (remaining < 4 * SIZE) ? prev_free : (size_t)res + size_allocated + SIZE;
	}
	if (remaining >= 4 * SIZE) {
		*(size_t*)(res + size_allocated + SIZE) = remaining;
		*(size_t*)(addr + size_free + SIZE) = remaining;
		*fl = res + size_allocated + SIZE;
	}
	else
		*(size_t*)fl = next_free;
	return (res);
}

void	*format_chunk_f(void *addr, void **fl, size_t s) {
	void* next_f = *fl;
	void* prev_f = NULL;
	if (*fl > addr) {
		*(size_t*)(addr + SIZE * 2) = (size_t)*fl;
		*(size_t*)(addr + SIZE) = (size_t)0;
		*fl = addr;
	}
	else {
		while (next_f != NULL && next_f < addr) {
			prev_f = next_f;
			next_f = (void *)*(size_t*)(next_f + 2 * SIZE);
		}
		*(size_t*)(addr + SIZE * 2) = (size_t)next_f;
		*(size_t*)(addr + SIZE) = (size_t)prev_f;
	}
	*(size_t*)(addr) = s;
	*(size_t*)(addr + s + SIZE) = s;
	return (addr);
}
