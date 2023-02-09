#include "../ft_malloc.h"

void	free_heap(victim_info_t victim) {

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
}

void    intern_free(void *ptr) {
	victim_info_t victim = get_ptr_info(ptr);
	if (victim.chunk == NULL)
		return;
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
