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
	size_t usable_size = pagesize - 3 * SIZE;
	*(size_t*)(new_page) = (size_t)usable_size;					// mark page start
	*(size_t*)(new_page + usable_size + SIZE) = (size_t)usable_size;	// mark page end
	*(size_t*)(new_page + usable_size + 2 * SIZE) = (size_t)0;			// next pointer
	hexdump(new_page, pagesize);
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

void	desallocate(void *ptr, void **fl, size_t size) {
	void *res;
	void **listpage;
	format_chunk_f(ptr, fl, size);
	res = free_merge_contiguous(ptr);
	if (GETSIZE(res) == (size_t)getpagesize() - 3 * SIZE) {
		listpage = (size < (size_t)getpagesize()/4) ? &b.lst_free_s : &b.lst_free_m;
		if ((size_t)*listpage == (size_t)res) {
			*listpage = (void *)*(size_t*)(res) + getpagesize();
		} else {
			void *cur = *listpage;
			while((void *)*(size_t*)(cur) + getpagesize() != res) {
				cur = (void *)*(size_t*)(cur) + getpagesize();
			}
			*(size_t*)(cur + getpagesize()) = *(size_t*)(res + getpagesize());
		}
		int test = munmap(res, getpagesize());
	}
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