#include "../ft_malloc.h"

/**
 *      page-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Back pointer to previous page in page list        |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *     `head:' |             Size of chunk, in bytes                     |A|0|1|
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             .                                                               .
 *             .                                                               .
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *   `footer:' |             Size of chunk, in bytes                     |A|0|1|
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Mark the end of page                        |0|0|1|
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *             |             Forward pointer to next page in page list         |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

void    *create_new_page() {
	int pagesize = getpagesize();
	void *new_page = mmap(
		NULL,
		pagesize,
		PROT_READ|PROT_WRITE, 
		MAP_SHARED|MAP_ANONYMOUS,
		-1, 0
	);
	if (new_page == MAP_FAILED)
		return (NULL);
	size_t usable_size = pagesize - 2 * SIZE;
	*(size_t*)(new_page) = (size_t)usable_size;					// mark page start
	*(size_t*)(new_page + usable_size) = (size_t)usable_size;	// next pointer
	*(size_t*)(new_page + usable_size + SIZE) = (size_t)0;
	hexdump(new_page, 60);
	return new_page;
}

void    *allocate(void **l, void **fl, size_t s) {
	void    *cur = *l;
	void    *res = NULL;
	void    *prev_page = NULL;
	cur = get_first_fit(*fl, s);
	if (cur == NULL) {
		prev_page = get_last_page_nxt_ptr(*l);
		cur = create_new_page();
		if (*l == NULL) {
			*l = cur;
			*fl = cur;
		} else {
			*(size_t*)prev_page = (size_t)cur;
		}
	}
	res = format_chunk_a(cur, fl, s);
	return (res);
}

void    *allocate_large(void** l, size_t s) {
	(void)s;
	(void)l;
	return (NULL);
}

void	free_merge_contiguous(void *ptr) {
	void* next_chunk_ptr = (ptr + GETSIZE(ptr) + 2 * SIZE);
	void* prev_chunk_ptr = (ptr - GETSIZE(ptr - SIZE) - 2 * SIZE);
	if (!ISALLOC(next_chunk_ptr))
	{
		*(size_t*)(ptr) = GETSIZE(ptr) + GETSIZE(next_chunk_ptr) + 2 * SIZE;
		*(size_t*)(ptr + GETSIZE(ptr) + SIZE) = GETSIZE(ptr);
		*(size_t*)(ptr + 2 * SIZE) = *(size_t*)(next_chunk_ptr + 2 * SIZE);
		for (size_t i = 0; i < GETSIZE(next_chunk_ptr); ++i)
			*(char*)(next_chunk_ptr - SIZE + i) = 0;
	}
	if (!ISALLOC(prev_chunk_ptr) && *(size_t*)(prev_chunk_ptr) != 0)
	{
		*(size_t*)(prev_chunk_ptr) = GETSIZE(ptr) + GETSIZE(prev_chunk_ptr) + 2 * SIZE;
		*(size_t*)(prev_chunk_ptr + GETSIZE(prev_chunk_ptr) + SIZE) = GETSIZE(prev_chunk_ptr);
		for (size_t i = 0; i < GETSIZE(ptr); ++i)
			*(char*)(ptr - SIZE + i) = 0;
	}
}

void	desallocate(void *ptr, void **fl, size_t size) {
	(void)size;
	format_chunk_f(ptr, fl, size);
	free_merge_contiguous(ptr);
}

void	desallocate_large(void *ptr, size_t size) {
	(void)ptr;
	(void)size;
}

void    *get_last_page_nxt_ptr(void* l)
{
	void *c = l;
	while (c != NULL) {
		c = (void *)*(size_t*)(c) + getpagesize() - 1;
	}
	return (c);
}