#include "../ft_malloc.h"

void    *get_first_fit(void* l, size_t s) {
	void *c = l;
	while (c != NULL && GETSIZE(c) < s) {
		c = (void*)get_value(c + FDPTR);
	}
	return (c);
}

void    *get_last_free(void** l) {
	void *c = *l;
	while (get_value(c + FDPTR)) {
		c = (void*)get_value(c + FDPTR);
	}
	return (c);
}

void hexdump(const void* data, size_t size) {
	if (data == NULL || size == 0)
		return;
	unsigned char *stream = (unsigned char*)data;
	char ascii[9];
	size_t i, j;
	ascii[8] = '\0';
	for (i = 0; i < size; ++i) {
        if (i % 8 == 0) {
			ft_putptr_fd((void *)(data + i), m.debug.output);
			ft_putstr_fd("  |  ", m.debug.output);
		}
		ft_puthex_char(stream[i], m.debug.output);
		ft_putstr_fd(" ", m.debug.output);
		if (stream[i] >= ' ' && stream[i] <= '~') {
			ascii[i % 8] = stream[i];
		} else {
			ascii[i % 8] = '.';
		}
		if ((i+1) % 4 == 0 || i+1 == size) {
			ft_putstr_fd(" ", m.debug.output);
			if ((i+1) % 8 == 0) {
				ft_putstr_fd("|  ", m.debug.output);
				ft_putstr_fd(ascii, m.debug.output);
				ft_putstr_fd(" \n", m.debug.output);
			} else if (i+1 == size) {
				ascii[(i+1) % 8] = '\0';
				if ((i+1) % 8 <= 4) {
					ft_putstr_fd(" ", m.debug.output);
				}
				for (j = (i+1) % 8; j < 8; ++j) {
					ft_putstr_fd("   ", m.debug.output);
				}
				ft_putstr_fd("|  ", m.debug.output);
				ft_putstr_fd(ascii, m.debug.output);
				ft_putstr_fd(" \n", m.debug.output);
			}
		}
	}
}

size_t	print_bucket(void *root) {
	size_t res = 0;
	while (root != NULL && GETSIZE(root) != 0)
    {
		ft_putptr_fd(root, m.debug.output);
		ft_putstr_fd(" - ", m.debug.output);
		ft_putptr_fd(root + GETSIZE(root) + SIZE, m.debug.output);
		ft_putstr_fd(" | ", m.debug.output);
		ft_putnbr_fd((int)GETSIZE(root), m.debug.output);
		ft_putstr_fd(" bytes", m.debug.output);
        if (ISALLOC(root)) {
			res += GETSIZE(root);
			ft_putstr_fd(" allocated\n", m.debug.output);
        } else {
			ft_putstr_fd(" free\n", m.debug.output);
        }
        root=(size_t*)(root + GETSIZE(root) + 2 * SIZE);
		if (get_value(root) == 0b01)
			root = (void *)get_value(root + FDPTR);
    }
	return (res);
}

