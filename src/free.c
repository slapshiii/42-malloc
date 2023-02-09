#include "../ft_malloc.h"

void	free_heap(victim_info_t victim) {
	if (victim.heap->bk)
		victim.heap->bk->fd = victim.heap->fd;
	if (victim.heap->fd)
		victim.heap->fd->bk = victim.heap->bk;
	if (m.heaplist[e_large] == victim.heap)
		m.heaplist[e_large] = victim.heap->fd;
	munmap(victim.heap, GETSIZE(victim.heap));
}

void	free_chunk(victim_info_t victim) {
	chunk_t *last = get_last_heap_block(victim.heap);
	chunk_t *next = NULL;
	if (victim.chunk != last)
		next = next_chunk(victim.chunk);
	CLEAR_INUSE(victim.chunk);
	if (!(victim.chunk->prev_size & PREV_INUSE)) {
		chunk_t	*prev = prev_chunk(victim.chunk);
		(victim.heap)->chk_cnt--;
		prev->size = victim.chunk->size + prev->size;
		victim.chunk = prev;
		if (next)
			next->prev_size = victim.chunk->size;
	}
	if (next) {
		next->prev_size &= ~(PREV_INUSE);
		if (!ISALLOC(next)) {
			(victim.heap)->chk_cnt--;
			victim.chunk->size = victim.chunk->size + next->size;
			if (next != last)
				next_chunk(next)->prev_size = victim.chunk->size;
		}
	}
	if (victim.heap->size == victim.chunk->size + sizeof(heap_t)) {
		int relink = 0;
		if (victim.heap->bk) {
			victim.heap->bk->fd = victim.heap->fd;
			relink = 1;
		}
		if (victim.heap->fd) {
			heap_t	**root = (victim.heap->size == TINY_ZONE_SIZE) ? &(m.heaplist[e_tiny]) : &(m.heaplist[e_small]);
			if (*root == victim.heap)
				*root = victim.heap->fd;
			victim.heap->fd->bk = victim.heap->bk;
			relink = 1;
		}
		if (relink) {
			munmap((void*)(victim.heap), victim.heap->size);
		}
	}
}

void    intern_free(void *ptr) {
	victim_info_t victim = get_ptr_info(ptr);
	if (victim.chunk == NULL) {
		ft_putendl_fd("Invalid ptr", 2);
		return;
	}
	if (ISMMAP(victim.heap)) {
		free_heap(victim);
	} else {
		free_chunk(victim);
	}
}

void    ft_free(void *ptr) {
	pthread_mutex_lock(&mutex_malloc);
	intern_free(ptr);
	pthread_mutex_unlock(&mutex_malloc);
}
