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

void    *get_first_fit(void* l, size_t s) {
	void *c = l;
	while (c != NULL && GETSIZE(c) < s) {
		c = (void *)*(size_t*)(c) + 2 * sizeof(void *);
	}
	return (c);
}

void	*free_merge_contiguous(void *ptr) {
	void* next_chunk_ptr = (ptr + GETSIZE(ptr) + 2 * SIZE);
	void* prev_chunk_ptr = (ptr - GETSIZE(ptr - SIZE) - 2 * SIZE);
	if (!ISALLOC(next_chunk_ptr))
	{
		*(size_t*)(ptr) = GETSIZE(ptr) + GETSIZE(next_chunk_ptr) + 2 * SIZE;
		*(size_t*)(ptr + GETSIZE(ptr) + SIZE) = GETSIZE(ptr);
		*(size_t*)(ptr + 2 * SIZE) = *(size_t*)(next_chunk_ptr + 2 * SIZE);
		for (size_t i = 0; i < SIZE * 4; i += SIZE)
			*(size_t*)(next_chunk_ptr - SIZE + i) = (size_t)0;
	}
	if (!ISALLOC(prev_chunk_ptr) && *(size_t*)(prev_chunk_ptr) != 0)
	{
		*(size_t*)(prev_chunk_ptr) = GETSIZE(ptr) + GETSIZE(prev_chunk_ptr) + 2 * SIZE;
		*(size_t*)(prev_chunk_ptr + GETSIZE(prev_chunk_ptr) + SIZE) = GETSIZE(prev_chunk_ptr);
		*(size_t*)(prev_chunk_ptr + 2 * SIZE) = *(size_t*)(ptr + 2 * SIZE);
		for (size_t i = 0; i < SIZE * 4; i += SIZE)
			*(size_t*)(ptr - SIZE + i) = (size_t)0;
		return (prev_chunk_ptr);
	}
	return (ptr);
}
