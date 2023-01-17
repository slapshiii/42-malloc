#include "../ft_malloc.h"

/**
 *      page-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
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
static void    *create_new_page() {
	int pagesize = getpagesize();
	void *new_page = mmap(
		NULL,
		pagesize,
		PROT_READ|PROT_WRITE, 
		MAP_SHARED|MAP_ANONYMOUS|MAP_POPULATE,
		-1, 0
	);
	if (new_page == MAP_FAILED)
		return (NULL);
	size_t usable_size = pagesize - 4 * SIZE;
	set_value(new_page							, usable_size);	// chunk header
	set_value(new_page + usable_size + SIZE		, usable_size);	// chunk footer
	set_value(new_page + usable_size + SIZE * 2	, 0b01);		// mark page end
	set_value(new_page + usable_size + SIZE * 3	, 0);			// nxt page ptr
	return new_page;
}

static void    *get_last_page_nxt_ptr(void** l)
{
	void *c = *l;
	if (c == NULL)
		return NULL;
	while (get_value(c + getpagesize() - SIZE) != 0)
		c = (void *)get_value(c + getpagesize() - SIZE);
	return (c + getpagesize() - SIZE);
}

void    *allocate(void **l, void **fl, size_t s) {
	void    *cur = *l;
	void    *res = NULL;
	void    *prev_page = NULL;
	void	*last_free = NULL;
	cur = get_first_fit(*fl, s);
	if (cur == NULL) {
		cur = create_new_page();
		if (*l == NULL) {
			*l = cur;
			*fl = cur;
		} else {
			prev_page = get_last_page_nxt_ptr(l);
			last_free = get_last_free(fl);
			set_value(prev_page, (size_t)cur);
			set_value(cur + BKPTR, (size_t)last_free);
			set_value(last_free + FDPTR, (size_t)cur);
		}
	}
	res = format_chunk_a(cur, fl, s);
	return (res);
}

/**
 *      page-> +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
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
 *             |             Backward pointer to previous page in page list    |
 *             +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */
void    *allocate_large(void** l, size_t s) {
	size_t	size_allocated;
	s += 5 * SIZE;
	if (s % SIZE)
		size_allocated = (s + SIZE) & ~(SIZE - 1);
	else
		size_allocated = s;
	void *new_page = mmap(
		NULL,
		size_allocated,
		PROT_READ|PROT_WRITE, 
		MAP_SHARED|MAP_ANONYMOUS,
		-1, 0
	);
	if (new_page == MAP_FAILED)
		return (NULL);
	size_t usable_size = size_allocated - 5 * SIZE;
	set_value(new_page							, usable_size + 0b011);	// chunk header
	set_value(new_page + usable_size + SIZE		, usable_size + 0b011);	// chunk footer
	set_value(new_page + usable_size + SIZE	* 2	, 0b01);				// padding
	set_value(new_page + usable_size + SIZE * 3	, (size_t) *l);			// nxt page ptr
	set_value(new_page + usable_size + SIZE * 4	, 0);					// prv page ptr
	if (*l != NULL)
		set_value(*l + GETSIZE(*l) + SIZE * 4, (size_t)new_page);
	*l = new_page;
	return new_page + SIZE;
}

void	desallocate(void *ptr, void **fl, size_t size) {
	void *res;
	void **listpage;
	size_t pagesize = (size_t)getpagesize();
	format_chunk_f(ptr, fl, size);
	res = free_merge_contiguous(ptr);
	if (GETSIZE(res) == pagesize - 4 * SIZE) {
		listpage = (size < pagesize/4) ? &b.lst_page_s : &b.lst_page_m;
		if (*listpage == res) {
			*listpage = (void *)get_value(res + pagesize - SIZE);
		} else {
			void *cur = *listpage;
			while((void *)get_value(cur + pagesize - SIZE) != res)
				cur = (void *)get_value(cur + pagesize - SIZE);
			set_value(cur + pagesize - SIZE, get_value(res + pagesize - SIZE));
		}

		if (*fl == res)
			*fl = (void *)get_value(res + FDPTR);
		if (get_value(res + FDPTR) != 0)
			set_value((void *)get_value(res + FDPTR) + BKPTR, get_value(res + BKPTR));
		if (get_value(res + BKPTR) != 0)
			set_value((void *)get_value(res + BKPTR) + FDPTR, get_value(res + FDPTR));
		munmap(res, pagesize);
	}
}

void	desallocate_large(void *ptr, size_t size) {
	void *prev, *next;
	next = (void *)get_value(ptr + size + SIZE * 3);
	prev = (void *)get_value(ptr + size + SIZE * 4);
	if (next != NULL)
		set_value(next + GETSIZE(next) + SIZE * 4, (size_t)prev);
	if (prev != NULL)
		set_value(prev + GETSIZE(prev) + SIZE * 3, (size_t)next);
	if (b.lst_page_l == ptr)
		b.lst_page_l = next;
	munmap(ptr, size + 5 * SIZE);
}
