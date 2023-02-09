#include "../ft_malloc.h"

void		abort_validate_ptr(void *ptr) {
	ft_putptr_fd(ptr - SIZE_SZ, m.debug.output);
	switch (m.debug.validate_ptrs)
	{
	case E_FREED:
		ft_putstr_fd(" - is already freed. Aborting\n", m.debug.output);
		break;
	case E_ALLOCATED:
		ft_putstr_fd(" - is not allocated. Aborting\n", m.debug.output);
		break;
	default:
		ft_putstr_fd(" - is not valid. Aborting\n", m.debug.output);
		break;
	}
	abort();
}

static int	validate_ptr_list(heap_t *root, void *ptr) {
	while (root != NULL)
	{
		if (ptr == root && ISALLOC(root)) {
			return (E_ALLOCATED);
		} else if (ptr == root && !ISALLOC(root)) {
			return (E_FREED);
		}
	}
	return (E_INVALID);
}

int			validate_ptr(void *ptr) {
	ptrs_val_t res[3];

	res[0] = validate_ptr_list(m.lst_page_s, ptr);
	res[1] = validate_ptr_list(m.lst_page_m, ptr);
	res[2] = validate_ptr_list(m.lst_page_l, ptr);
	if (res[0] == E_ALLOCATED || res[1] == E_ALLOCATED || res[2] == E_ALLOCATED) {
		return (1);
	}
	else if (res[0] == E_FREED || res[1] == E_FREED || res[2] == E_FREED)
		m.debug.validate_ptrs = (m.debug.validate_ptrs == E_OFF) ? E_OFF : E_FREED;
	else
		m.debug.validate_ptrs = (m.debug.validate_ptrs == E_OFF) ? E_OFF : E_INVALID;
	return (0);
}

static void	report_allocation(heap_t *root) {
	while (root != NULL)
    {
        if (ISALLOC(root)) {
			ft_putptr_fd(root, m.debug.output);
			ft_putstr_fd(" - ", m.debug.output);
			ft_putptr_fd(root + GETSIZE(root), m.debug.output);
			ft_putstr_fd(" | ", m.debug.output);
			ft_putnbr_fd((int)GETSIZE(root), m.debug.output);
			ft_putstr_fd(" bytes", m.debug.output);
			ft_putstr_fd(" allocated\n", m.debug.output);
        }
    }
}

void	report_allocations(void) {
	ft_putstr_fd("The list of allocations still active:\n", m.debug.output);
	report_allocation(m.lst_page_s);
	report_allocation(m.lst_page_m);
	report_allocation(m.lst_page_l);
}

void	fill_pattern(void *addr, char *pattern, size_t size) {
	char *ptr = (char *)addr;
	size_t len_pattern = ft_strlen(pattern);
	for (size_t i = 0; i < size; ++i) {
		ptr[i] = pattern[i % len_pattern];
	}
}
