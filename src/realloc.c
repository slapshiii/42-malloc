#include "../ft_malloc.h"

void    *intern_realloc(void *ptr, size_t size) {
	if (size == 0 && ptr) {
		intern_free(ptr);
		return (NULL);
	}
	size = get_align_size(size);
	if (!ptr)
		return(intern_malloc(size));
	victim_info_t victim = get_ptr_info(ptr);
	if (check_ptr(victim, ptr))
		return (NULL);
	if (victim.chunk->size == size)
		return (ptr);
	void *res = NULL;
	if (get_zone_size(victim.size) == (size_t)-1 || get_zone_size(size) != get_zone_size(victim.size)) {
		res = intern_malloc(size);
		if (res) {
			ft_memmove(res, ptr, ((victim.size > size) ? size : victim.size) - 2*SIZE_SZ);
			intern_free(ptr);
		}
		return (res);
	}
	if (victim.chunk != get_last_heap_block(victim.heap)) {
		chunk_t *next = next_chunk(victim.chunk);
		size_t sizefree = victim.size + GETSIZE(next);
		if (!ISALLOC(next) && sizefree >= size) {
			if (sizefree - size >= MINSIZE) {
				chunk_t *new_chunk = (void*)(victim.chunk) + size;
				init_chunk(new_chunk, sizefree - size, 0, size + 1);
				victim.chunk->size = size + 1;
				new_chunk->bk = next->bk;
				new_chunk->fd = next->fd;
			} else {
				victim.heap->chk_cnt--;
				victim.chunk->size = sizefree + 1;
				if (next != get_last_heap_block(victim.heap))
					next_chunk(next)->prev_size = sizefree + 1;
			}
			return (ptr);
		}
	}
	res = intern_malloc(size);
	if (res) {
		ft_memmove(res, ptr, ((victim.size > size) ? size : victim.size) - 2*SIZE_SZ);
		intern_free(ptr);
	}
	return (res);
}

void    *realloc(void *ptr, size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	ptr = intern_realloc(ptr, size);
	pthread_mutex_unlock(&mutex_malloc);
	return (ptr);
}
