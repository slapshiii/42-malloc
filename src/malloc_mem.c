#include "../ft_malloc.h"

/**
 *     chunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Size of previous chunk, if unallocated (P clear)  |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     `head:' |             Size of chunk, in bytes                     |A|0|P|
 *       mem-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Forward pointer to next chunk in freelist         |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Back pointer to previous chunk in freelist        |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Unused space (may be 0 bytes long)                .
 *             .                                                               .
 *             .                                                               |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     `foot:' |             Size of chunk, in bytes                           |
 * nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Size of next chunk, in bytes                |A|0|0|
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

void	*format_chunk_a(void *addr, void **fl, size_t s) {
	size_t	prev_free = get_value(addr + BKPTR);
	size_t	next_free = get_value(addr + FDPTR);

	void	*res = (void *)(addr + SIZE);

	size_t	size_free = GETSIZE(addr);
	size_t	size_allocated;
	if (s < MINSIZE)
		size_allocated = MINSIZE;
	else if (s % SIZE)
		size_allocated = (s + SIZE) & ~(SIZE - 1);
	else
		size_allocated = s;
	int remaining = size_free - size_allocated - 2 * SIZE;
	if (remaining < (int)(2 * SIZE)) {
		size_allocated = size_free;
	}

	set_value(addr, size_allocated + 1);
	set_value(res + size_allocated, size_allocated + 1);
	if ((void*)prev_free != NULL){
		set_value((void*)(prev_free + FDPTR),
			(remaining < (int)(4 * SIZE)) ? next_free : (size_t)res + size_allocated + SIZE
		);
	}
	if ((void*)next_free != NULL) {
		set_value((void*)(next_free + BKPTR),
			(remaining < (int)(4 * SIZE)) ? prev_free : (size_t)res + size_allocated + SIZE
		);
	}
	if (remaining >= (int)(2 * SIZE)) {
		set_value(res + size_allocated + SIZE, remaining);
		set_value(addr + size_free + SIZE, remaining);
	}
	if (*fl == addr)
		*fl = (remaining >= (int)(2 * SIZE)) ? res + size_allocated + SIZE : (void*)next_free;
	return (res);
}

void	*format_chunk_f(void *addr, void **fl, size_t s) {
	void* next_f = *fl;
	void* prev_f = NULL;
	if (*fl > addr) {
		set_value(addr + FDPTR, (size_t)*fl);
		set_value(addr + BKPTR, 0);
		*fl = addr;
	}
	else {
		while (next_f != NULL && next_f < addr) {
			prev_f = next_f;
			next_f = (void *)get_value(next_f + FDPTR);
		}
		set_value(addr + FDPTR, (size_t)next_f);
		set_value(addr + BKPTR, (size_t)prev_f);
		if (prev_f != NULL)
			set_value(prev_f + FDPTR, (size_t)addr);
	}
	set_value(addr, s);
	set_value(addr + s + SIZE, s);
	return (addr);
}
