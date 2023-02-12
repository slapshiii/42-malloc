#include "../ft_malloc.h"

int		init_malloc() {
	if (m.heaplist[e_tiny] == NULL) {
		m.heaplist[e_tiny] = create_heap(TINY_ZONE_SIZE, NULL);
		if (m.heaplist[e_tiny] == NULL)
			return (1);
	}
	if (m.heaplist[e_small] == NULL) {
		m.heaplist[e_small] = create_heap(SMALL_ZONE_SIZE, NULL);
		if (m.heaplist[e_small] == NULL)
			return (1);
	}
	return (0);
}

void    *allocate(heap_t **l, size_t s) {
	victim_info_t	victim;
	victim = get_first_fit(*l, s);
	if (victim.chunk == NULL) {
		heap_t *last = get_last_heap(*l);
		heap_t *new_heap = create_heap(last->size, last);
		if (new_heap == NULL)
			return (NULL);
		if (last)
			last->fd = new_heap;
		victim = (victim_info_t){new_heap, heap2chunk(new_heap), heap2chunk(new_heap)->size};
	}
	if (victim.size - s >= MINSIZE) {
		chunk_t *new_chunk = (void*)(victim.chunk) + s;
		new_chunk->prev_size = s | PREV_INUSE;
		new_chunk->size = victim.size - s;
		new_chunk->bk = victim.chunk->bk;
		new_chunk->fd = victim.chunk->fd;
		init_chunk(victim.chunk, s, PREV_INUSE, victim.chunk->prev_size);
		victim.heap->chk_cnt++;
	} else {
		victim.chunk->size++;
	}
	return (chunk2mem(victim.chunk));
}

void    *allocate_mmap(heap_t **l, size_t s) {
	heap_t *last = get_last_heap(*l);
	heap_t *new_heap = mmap(
		NULL, s,
		PROT_READ|PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS,
		-1, 0
	);
	if (!new_heap)
		return (NULL);
	if (!last) {
		*l = new_heap;
		new_heap->chk_cnt = 0;
	} else {
		last->fd = new_heap;
		new_heap->chk_cnt = last->chk_cnt + 1;
	}
	new_heap->size = s | PREV_INUSE | IS_MMAPPED;
	new_heap->bk = last;
	new_heap->fd = NULL;
	fill_pattern(heap2chunk(new_heap), m.debug.pattern_alloc, GETSIZE(new_heap) - sizeof(heap_t));
	return (heap2chunk(new_heap));
}

void	*intern_malloc(size_t size) {
	void    *res = NULL;
	if (size == 0)
		return (NULL);
	if (init_malloc()){
		ft_putendl_fd("Could not initiate zones", 2);
		return (NULL);
	}
	size = get_align_size(size);
	if (size <= TINY_CHUNK_SIZE) {
		res = allocate(&m.heaplist[e_tiny], size);
	} else if (size <= SMALL_CHUNK_SIZE) {
		res = allocate(&m.heaplist[e_small], size);
	} else {
		res = allocate_mmap(&m.heaplist[e_large], size + MALLOC_ALIGNMENT);
	}
	return res;
}

void    *malloc(size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	void    *res = NULL;
	res = intern_malloc(size);
	pthread_mutex_unlock(&mutex_malloc);
	return (res);
}
