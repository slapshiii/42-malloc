#include "../ft_malloc.h"

void init_chunk(chunk_t *addr, size_t size, int free, size_t prev_sz) {
	addr->prev_size = prev_sz;
	addr->size = size + free;
	if (free) {
		addr->fd = NULL;
		addr->bk = NULL;
		// if (ft_strlen(m.debug.pattern_free))
		// 	fill_pattern(addr + sizeof(chunk_t), m.debug.pattern_free, size - sizeof(chunk_t));
	} else {
		// if (ft_strlen(m.debug.pattern_alloc))
		// 	fill_pattern(chunk2mem(addr), m.debug.pattern_alloc, size - 2*SIZE_SZ);
	}
}

void	merge_next_chunk(chunk_t *chunk) {
	chunk_t	*next = next_chunk(chunk);
	chunk->size = chunk->size + next->size + sizeof(chunk_t);
	chunk->fd = next->fd;
}
