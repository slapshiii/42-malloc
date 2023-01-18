#include "../ft_malloc.h"

void	report_allocations_option(const char *option) {
	char *start = ft_strnstr(option, "report_allocations", ft_strlen(option));
	if (start != NULL) {
		m.debug.report_allocations = 1;
	} else
		m.debug.report_allocations = 0;
}

void	validate_ptrs_option(const char *option) {
	char *start = ft_strnstr(option, "validate_ptrs", ft_strlen(option));
	if (start != NULL) {
		m.debug.validate_ptrs = 1;
	} else
		m.debug.validate_ptrs = 0;
}

void	output_option(const char *option) {
	char *start = ft_strnstr(option, "output:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("output:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		char buf[end-start + 1];
		ft_strlcpy(buf, start, end - start + 1);
		if (ft_strcmp(buf, "stderr") == 0)
			m.debug.output = 2;
		else if (ft_strcmp(buf, "stdout") == 0)
			m.debug.output = 1;
		else
			m.debug.output = open(buf, O_CREAT|O_WRONLY|O_TRUNC);
	} else
		m.debug.output = 2;
}

void	pattern_alloc_option(const char *option) {
	char *start = ft_strnstr(option, "fill:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("fill:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		ft_strlcpy(m.debug.pattern_alloc, start, end - start + 1);
	} else
		bzero(m.debug.pattern_alloc, 128);
}

void	pattern_free_option(const char *option) {
	char *start = ft_strnstr(option, "free_fill:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("free_fill:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		ft_strlcpy(m.debug.pattern_free, start, end - start + 1);
	} else
		bzero(m.debug.pattern_free, 128);
}

void		abort_validate_ptr(int status, void *ptr) {
	ft_putptr_fd(ptr - SIZE, m.debug.output);
	switch (status)
	{
	case 0:
		ft_putstr_fd(" - is already freed. Aborting\n", m.debug.output);
		break;
	case 1:
		ft_putstr_fd(" - is not allocated. Aborting\n", m.debug.output);
		break;
	default:
		ft_putstr_fd(" - is not valid. Aborting\n", m.debug.output);
		break;
	}
	abort();
}

int			validate_ptr(void *root, void *ptr) {
	while (root != NULL && GETSIZE(root) != 0)
    {
        if (ptr == root && ISALLOC(root)) {
			return (1);
        } else if (ptr == root && !ISALLOC(root)) {
			return (0);
		}
        root=(size_t*)(root + GETSIZE(root) + 2 * SIZE);
		if (get_value(root) == 0b01)
			root = (void *)get_value(root + FDPTR);
    }
	return (0);
}

static void	report_allocation(void *root) {
	while (root != NULL && GETSIZE(root) != 0)
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
        root=(size_t*)(root + GETSIZE(root) + 2 * SIZE);
		if (get_value(root) == 0b01)
			root = (void *)get_value(root + FDPTR);
    }
}

void	report_allocations(void) {
	ft_putstr_fd("The list of allocations still active:\n", m.debug.output);
	report_allocation(m.lst_page_s);
	report_allocation(m.lst_page_m);
	report_allocation(m.lst_page_l);
}
