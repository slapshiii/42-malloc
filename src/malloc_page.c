#include "../ft_malloc.h"

void    *create_new_page(void *prev_free) {
	int pagesize = getpagesize();
	void *new_page = mmap(
		NULL,
		pagesize,
		PROT_READ|PROT_WRITE, 
		MAP_SHARED|MAP_ANONYMOUS,
		-1, 0
	);
	printf("%p -- create new page\n%d -- size\n", new_page, pagesize);
	if (new_page == MAP_FAILED)
		return (NULL);
	size_t usable_size = pagesize - 2 * sizeof(int);
	*(size_t*)(new_page) = usable_size;
	*(size_t*)(new_page + SIZE) = (size_t)prev_free;
	*(size_t*)(new_page + 2 * SIZE) = 0x0;
	*(size_t*)(new_page + 3 * SIZE) = 0x42424242;
	*(size_t*)(new_page + usable_size) = usable_size;
	printf("%p -- create new page\n", new_page);
	printf("%lX -- %p\n", *(size_t*)new_page, new_page);
	printf("%lX -- %p\n", *(size_t*)(new_page + SIZE), new_page + SIZE);
	printf("%lX -- %p\n", *(size_t*)(new_page + 2 * SIZE), new_page + 2 * SIZE);
	printf("%lX -- %p\n", *(size_t*)(new_page + 3 * SIZE), new_page + 3 * SIZE);
	printf("%lX -- %p\n", *(size_t*)(new_page + usable_size), new_page + usable_size);
	return new_page;
}

void    *get_first_fit(void* l, size_t s, void **prev_free) {
	void *c = l;
	while (c != NULL && ISALLOC(c)) {
		c = c + (*(int*)(c) & (~7));
	}
	while (c != NULL && GETSIZE(c) < s) {
		c = (void *)*(size_t*)(c) + 2 * sizeof(void *);
		*prev_free = c;
	}
	return (c);
}

void    *allocate(void **l, size_t s) {
	void    *cur = *l;
	void    *res = NULL;
	void    *prev_free = NULL;
	cur = get_first_fit(cur, s, &prev_free);
	if (cur == NULL) {
		cur = create_new_page(prev_free);
		*l = cur;
	}
	//printf("%p -- before format\n", cur);
	//res = format_chunk_a(cur, s);
	return (res);
}

void    *allocate_large(void** l, size_t s) {
	(void)s;
	(void)l;
	return (NULL);
}

void	*format_chunk_a(void *addr, size_t s) {
	
	void	*prev_free = (void *)(addr + SIZE);
	void	*next_free = (void *)(addr + SIZE * 2);
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
	*(size_t*)(addr) = size_allocated + 1;
	*(size_t*)(res + size_allocated) = size_allocated + 1;
	*(size_t*)(prev_free + SIZE * 2) = (size_t)addr;
	*(size_t*)(res + size_allocated + SIZE) = remaining;
	*(size_t*)(res + size_allocated + 2 * SIZE) = (size_t)next_free;
	*(size_t*)(addr + size_free) = remaining;
	return (res);
}

