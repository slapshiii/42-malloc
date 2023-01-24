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

	int remaining = size_free - s - 2 * SIZE;
	if (remaining < (int)(2 * SIZE)) {
		s = size_free;
	}
	set_value(addr, s + 1);
	set_value(res + s, s + 1);
	if ((void*)prev_free != NULL){
		set_value((void*)(prev_free + FDPTR),
			(remaining < (int)(4 * SIZE)) ? next_free : (size_t)res + s + SIZE
		);
	}
	if ((void*)next_free != NULL) {
		set_value((void*)(next_free + BKPTR),
			(remaining < (int)(4 * SIZE)) ? prev_free : (size_t)res + s + SIZE
		);
	}
	if (remaining >= (int)(2 * SIZE)) {
		set_value(res + s + SIZE, remaining);
		set_value(addr + size_free + SIZE, remaining);
		set_value(res + s + SIZE + FDPTR, next_free);
		set_value(res + s + SIZE + BKPTR, prev_free);
	}
	if (*fl == addr)
		*fl = (remaining >= (int)(2 * SIZE)) ? res + s + SIZE : (void*)next_free;
	if (ft_strlen(m.debug.pattern_alloc))
		fill_pattern(addr + SIZE, m.debug.pattern_alloc, GETSIZE(addr));
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
		if (next_f != NULL)
			set_value(next_f + BKPTR, (size_t)addr);
	}
	set_value(addr, s);
	set_value(addr + s + SIZE, s);

	if (ft_strlen(m.debug.pattern_free))
		fill_pattern(addr + 3 * SIZE, m.debug.pattern_free, GETSIZE(addr) - 2 * SIZE);
	return (addr);
}

void	*free_merge_contiguous(void *ptr) {
	void* next_chunk_ptr = (ptr + GETSIZE(ptr) + 2 * SIZE);
	if (!ISALLOC(next_chunk_ptr))
	{
		void *ptr_footer = ptr + GETSIZE(ptr) + GETSIZE(next_chunk_ptr) + 3 * SIZE;
		set_value(ptr			, GETSIZE(ptr) + GETSIZE(next_chunk_ptr) + 2 * SIZE);
		set_value(ptr_footer	, GETSIZE(ptr));
		set_value(ptr + FDPTR	, get_value(next_chunk_ptr + FDPTR));
		set_value(ptr + BKPTR	, get_value(next_chunk_ptr + BKPTR));
		if (ft_strlen(m.debug.pattern_free))
			fill_pattern(next_chunk_ptr - SIZE, m.debug.pattern_free, SIZE * 4);
	}
	void *prev_chunk_footer = (ptr - SIZE);
	if ((size_t)ptr & (0x0FFF) && !ISALLOC(prev_chunk_footer)) 
	{
		void *prev_chunk_ptr = prev_chunk_footer - GETSIZE(prev_chunk_footer) - SIZE;
		set_value(prev_chunk_ptr			, GETSIZE(ptr) + GETSIZE(prev_chunk_ptr) + 2 * SIZE);
		set_value(prev_chunk_footer			, GETSIZE(prev_chunk_ptr));
		set_value(prev_chunk_ptr + FDPTR	, get_value(ptr + FDPTR));
		if (ft_strlen(m.debug.pattern_free))
			fill_pattern(ptr - SIZE, m.debug.pattern_free, SIZE * 4);
		return (prev_chunk_ptr);
	}
	return (ptr);
}

/**
 *	Tries to extend the allocated chunk with contiguous free chunk
 *	@param ptr	: the chunk pointer
 *	@param size	: the desired size
 *	@return int	: 0 in case of success, 1 otherwise
 */
int		try_extend_chunk(void *ptr, size_t size) {
	size_t oldsize = GETSIZE(ptr - SIZE);
	void *ptr_next = ptr + oldsize + SIZE;
	size_t free_size = oldsize + GETSIZE(ptr_next) + 2 * SIZE;
	size_t realloc_size = oldsize + GETSIZE(ptr_next) + 2 * SIZE;
	size_t f_bucket = 1;
	void **fl = NULL;

	if (size < MINSIZE)
		size = MINSIZE;
	else if (size % SIZE)
		size = (size + SIZE) & ~(SIZE - 1);
	if (oldsize == size)
		return (0);
	if (size < m.pagesize/4 && oldsize < m.pagesize/4) {
		fl = &m.lst_free_s;
		f_bucket = 0;
	}
	else if (size >= m.pagesize/4 && size < m.pagesize && oldsize < m.pagesize) {
		fl = &m.lst_free_m;
        f_bucket = 0;
	}
	if (ISALLOC(ptr_next) || realloc_size < size || f_bucket)
		return (1);
	if (free_size - size >= MINSIZE) {
		free_size = free_size - size - 2 * SIZE;
		realloc_size = realloc_size - free_size - 2 * SIZE;
		set_value(ptr + realloc_size + 3 * SIZE	, get_value(ptr_next + BKPTR));
		set_value(ptr + realloc_size + 2 * SIZE	, get_value(ptr_next + FDPTR));
		set_value(ptr + realloc_size + SIZE		, free_size);
		set_value(ptr + realloc_size + free_size, free_size);
		if (*fl == ptr_next)
			*fl = ptr + realloc_size + SIZE;
	} else {
		if (get_value(ptr_next + FDPTR))
			set_value((void *)get_value(ptr_next + FDPTR) + BKPTR	, get_value(ptr_next + BKPTR));
		if (get_value(ptr_next + BKPTR))
			set_value((void *)get_value(ptr_next + BKPTR) + FDPTR	, get_value(ptr_next + FDPTR));
		if (*fl == ptr_next)
			*fl = (void *)get_value(ptr_next + FDPTR);
	}
	set_value(ptr - SIZE			, realloc_size + 0b001);
	set_value(ptr + realloc_size 	, realloc_size + 0b001);
	return (0);
}

void	set_value(void *addr, size_t val) {
	*(size_t*)(addr) = val;
}

size_t	get_value(void *addr) {
	return *(size_t*)(addr);
}
