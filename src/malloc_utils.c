#include "../ft_malloc.h"

victim_info_t	get_first_fit(heap_t* l, size_t s) {
	heap_t *h = l;
	chunk_t *c = NULL;
	while (h != NULL) {
		c = heap2chunk(h);
		INTERNAL_SIZE_T i = 0;
		while (i < h->chk_cnt) {
			if (ISALLOC(c)) {
				++i;
				c = next_chunk(c);
				continue;
			}
			while (GETSIZE(c) < s && i < h->chk_cnt - 1 && !ISALLOC(next_chunk(c)))
			{
				merge_next_chunk(c); //defragment
				--h->chk_cnt;
			}
			if (GETSIZE(c) >= s)
				return ((victim_info_t){h, c, c->size});
			++i;
			c = next_chunk(c);
		}
		h = h->fd;
	}
	return ((victim_info_t){NULL, NULL, 0});
}

size_t	get_align_size(const size_t size) {
	size_t	res = (size + (MALLOC_ALIGN_MASK)) & ~(MALLOC_ALIGN_MASK);
	res = ((res) + SIZE_SZ + MALLOC_ALIGN_MASK < MINSIZE) ? MINSIZE :                                                      \
		((res) + SIZE_SZ + MALLOC_ALIGN_MASK) & ~MALLOC_ALIGN_MASK;
	return (res);
}

size_t	get_zone_size(size_t size) {
	if (size <= TINY_CHUNK_SIZE)
		return (TINY_ZONE_SIZE);
	else if (size <= SMALL_CHUNK_SIZE)
		return (SMALL_ZONE_SIZE);
	return (-1);
}

victim_info_t get_ptr_info(void *ptr) {
	for (INTERNAL_SIZE_T i = 0; i < 3; ++i) {
		heap_t *root = m.heaplist[i];
		while (root != NULL)
		{
			if (ISMMAP(root) && (void *)heap2chunk(root) == ptr) {
				return ((victim_info_t){root, heap2chunk(root), GETSIZE(root)});
			} else {
				chunk_t *chunk = heap2chunk(root);
				for (INTERNAL_SIZE_T i = 0; i < root->chk_cnt; ++i) {
					if (chunk2mem(chunk) == ptr){
						return ((victim_info_t){root, chunk, GETSIZE(chunk)});
					}

					chunk = next_chunk(chunk);
				}
			}
			root = root->fd;
		}
	}
	return ((victim_info_t){NULL, NULL, 0});
}
