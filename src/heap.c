#include "../ft_malloc.h"

heap_t	*create_heap(size_t s, heap_t *prev_heap) {
	heap_t *new_heap = mmap(
		NULL, s,
		PROT_READ|PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS,
		-1, 0
	);
	if (new_heap == MAP_FAILED){
		return (NULL);
	}
	new_heap->chk_cnt = 1;
	new_heap->size = s;
	new_heap->fd = NULL;
	new_heap->bk = prev_heap;
	if (prev_heap)
		prev_heap->fd = new_heap;
	init_chunk(heap2chunk(new_heap), s - sizeof(heap_t), 0, 1);
	return new_heap;
}

heap_t	*get_last_heap(heap_t *root) {
	heap_t	*cur = root;
	while (cur && cur->fd)
	{
		cur = cur->fd;
	}
	return (cur);	
}

chunk_t	*get_last_heap_block(heap_t *root) {
	if (ISMMAP(root))
		return(NULL);
	chunk_t *chunk = heap2chunk(root);
	for (INTERNAL_SIZE_T i = 0; i < root->chk_cnt - 1; ++i) {
		chunk = next_chunk(chunk);
	}
	return (chunk);	
}
