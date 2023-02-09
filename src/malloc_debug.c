#include "../ft_malloc.h"

int			check_ptr(victim_info_t victim, void *ptr) {
	if (victim.chunk == NULL) {
		ft_putptr_fd(ptr, m.debug.output);
		ft_putstr_fd(" - is not valid.", m.debug.output);
		if (m.debug.validate_ptrs == E_ON) {
			ft_putendl_fd(" Aborting...", m.debug.output);
			pthread_mutex_unlock(&mutex_malloc);
			abort();
		}
		else {
			ft_putendl_fd(" Warning!", m.debug.output);
			return (1);
		}
	}
	if (!ISALLOC(victim.chunk)) {
		ft_putptr_fd(ptr, m.debug.output);
		ft_putstr_fd(" - is already freed.", m.debug.output);
		if (m.debug.validate_ptrs == E_ON) {
			ft_putendl_fd(" Aborting...", m.debug.output);
			pthread_mutex_unlock(&mutex_malloc);
			abort();
		}
		else {
			ft_putendl_fd(" Warning!", m.debug.output);
			return (1);
		}
	}
	return (0);
}

static void	report_allocation(heap_t *root) {
	while (root != NULL)
    {
		if (ISMMAP(root)) {
			if (ISALLOC(root)) {
			ft_putptr_fd(root, m.debug.output);
			ft_putstr_fd(" - ", m.debug.output);
			ft_putptr_fd(root + GETSIZE(root), m.debug.output);
			ft_putstr_fd(" | ", m.debug.output);
			ft_putnbr_fd((int)GETSIZE(root), m.debug.output);
			ft_putstr_fd(" bytes", m.debug.output);
			ft_putstr_fd(" allocated\n", m.debug.output);
        }
		} else {
			chunk_t *chunk = heap2chunk(root);
			for (INTERNAL_SIZE_T i = 0; i < root->chk_cnt; ++i) {
				if (ISALLOC(chunk)){
					ft_putptr_fd(chunk, m.debug.output);
					ft_putstr_fd(" - ", m.debug.output);
					ft_putptr_fd(chunk + GETSIZE(chunk), m.debug.output);
					ft_putstr_fd(" | ", m.debug.output);
					ft_putnbr_fd((int)GETSIZE(chunk), m.debug.output);
					ft_putstr_fd(" bytes", m.debug.output);
					ft_putstr_fd(" allocated\n", m.debug.output);
				}
				chunk = next_chunk(chunk);
			}
		}
		root = root->fd;
    }
}

void	report_allocations(void) {
	ft_putstr_fd("The list of allocations still active:\n", m.debug.output);
	for (INTERNAL_SIZE_T i = 0; i < 3; ++i) {
		report_allocation(m.heaplist[i]);
	}
}

void	fill_pattern(void *addr, char *pattern, size_t size) {
	size_t len_pattern = 0;
	if ((len_pattern = ft_strlen(pattern))) {
		char *ptr = (char *)addr;
		for (size_t i = 0; i < size; ++i) {
			ptr[i] = pattern[i % len_pattern];
		}
	}
}
