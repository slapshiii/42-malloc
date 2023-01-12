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
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     `foot:' |             Size of chunk, in bytes                           |
 * nextchunk-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Size of next chunk, in bytes                |A|0|P|
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

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

void	*free_merge_contiguous(void *ptr) {
	void* next_chunk_ptr = (ptr + GETSIZE(ptr) + 2 * SIZE);
	if (get_value(ptr + FDPTR) == (size_t)next_chunk_ptr)
	{
		void *ptr_footer = ptr + SIZE + GETSIZE(ptr);
		set_value(ptr			, GETSIZE(ptr) + GETSIZE(next_chunk_ptr) + 2 * SIZE);
		set_value(ptr_footer	, GETSIZE(ptr));
		set_value(ptr + FDPTR	, get_value(next_chunk_ptr + FDPTR));

		for (size_t i = 0; i < SIZE * 4; i += SIZE)
			set_value(next_chunk_ptr - SIZE + i, 0);
	}
	if ((size_t)ptr & (0x0FFF)) {
		void* prev_chunk_ptr = (ptr - GETSIZE(ptr - SIZE) - 2 * SIZE);
		if (get_value(ptr + BKPTR) == (size_t)prev_chunk_ptr)
		{
			void *prev_chunk_footer = prev_chunk_ptr + GETSIZE(prev_chunk_ptr) + SIZE;
			set_value(prev_chunk_ptr			, GETSIZE(ptr) + GETSIZE(prev_chunk_ptr) + 2 * SIZE);
			set_value(prev_chunk_footer			, GETSIZE(prev_chunk_ptr));
			set_value(prev_chunk_ptr + FDPTR	, get_value(ptr + FDPTR));

			for (size_t i = 0; i < SIZE * 4; i += SIZE)
				set_value(ptr - SIZE + i, 0);
			return (prev_chunk_ptr);
		}
	}
	return (ptr);
}

size_t	print_bucket(void *root) {
	size_t res = 0;
	while (root != NULL && GETSIZE(root) != 0)
    {
        printf("%lX - %lX | %ld bytes",
                (unsigned long)root, (unsigned long)(root + GETSIZE(root)), GETSIZE(root));
        if (ISALLOC(root)) {
			res += GETSIZE(root);
            printf(" allocated\n");
        } else {
            printf(" free\n");
        }
        root=(size_t*)(root + GETSIZE(root) + 2 * SIZE);
		if (get_value(root) == 0b01)
			root = (void *)get_value(root + FDPTR);
    }
	return (res);
}

void	set_value(void *addr, size_t val) {
	*(size_t*)(addr) = val;
}

size_t	get_value(void *addr) {
	return *(size_t*)(addr);
}
