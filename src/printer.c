#include "../ft_malloc.h"

void    ft_putptr_fd(void *p, int fd) {
    unsigned char buf[sizeof(void *)];
    ft_memmove(buf, &p, sizeof(void *));
    write(fd, "0x", 2);
    for (int i = sizeof(void *) - 1; i >= 0; i--) {
        unsigned char hi = (buf[i] >> 4) & 0x0f; 
        unsigned char lo = buf[i] & 0x0f;        
        char tmp[2] = {hi, lo};

        tmp[0] += hi < 10 ? '0' : 'A' - 10;
        tmp[1] += lo < 10 ? '0' : 'A' - 10;

        write(fd, tmp, 2);
    }
}

void    ft_puthex_char(unsigned char c, int fd) {
    unsigned char hi = (c >> 4) & 0x0f; 
    unsigned char lo = c & 0x0f;        
    char tmp[2] = {hi, lo};

    tmp[0] += (hi < 10) ? '0' : 'A' - 10;
    tmp[1] += (lo < 10) ? '0' : 'A' - 10;

    write(fd, tmp, 2);
}


void hexdump(const void* data, size_t size) {
	if (data == NULL || size == 0)
		return;
	unsigned char *stream = (unsigned char*)data;
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
        if (i % 16 == 0) {
			ft_putptr_fd((void *)(data + i), m.debug.output);
			ft_putstr_fd("  |  ", m.debug.output);
		}
		ft_puthex_char(stream[i], m.debug.output);
		ft_putstr_fd(" ", m.debug.output);
		if (stream[i] >= ' ' && stream[i] <= '~') {
			ascii[i % 16] = stream[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			ft_putstr_fd(" ", m.debug.output);
			if ((i+1) % 16 == 0) {
				ft_putstr_fd("|  ", m.debug.output);
				ft_putstr_fd(ascii, m.debug.output);
				ft_putstr_fd(" \n", m.debug.output);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					ft_putstr_fd(" ", m.debug.output);
				}
				for (j = (i+1) % 16; j < 16; ++j) {
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
		ft_putptr_fd(root + GETSIZE(root) + SIZE - 1, m.debug.output);
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

void    show_alloc_mem() {
	pthread_mutex_lock(&mutex_malloc);
	size_t total = 0;
	ft_putstr_fd("TINY : ", m.debug.output);
	ft_putptr_fd(m.lst_page_s, m.debug.output);
	ft_putstr_fd("\n", m.debug.output);
	total += print_bucket(m.lst_page_s);
	ft_putstr_fd("SMALL : ", m.debug.output);
	ft_putptr_fd(m.lst_page_m, m.debug.output);
	ft_putstr_fd("\n", m.debug.output);
	total += print_bucket(m.lst_page_m);
	ft_putstr_fd("LARGE : ", m.debug.output);
	ft_putptr_fd(m.lst_page_l, m.debug.output);
	ft_putstr_fd("\n", m.debug.output);
	total += print_bucket(m.lst_page_l);
	ft_putstr_fd("Total : ", m.debug.output);
	ft_putnbr_fd(total, m.debug.output);
	ft_putstr_fd(" bytes\n", m.debug.output);
	pthread_mutex_unlock(&mutex_malloc);
}

void    show_alloc_mem_hex(void *ptr) {
	pthread_mutex_lock(&mutex_malloc);
	if (validate_ptr(ptr - SIZE) == 0) {
		if (m.debug.validate_ptrs == E_OFF) {
			pthread_mutex_unlock(&mutex_malloc);
			return;
		} else
			abort_validate_ptr(ptr);
	}
	hexdump(ptr, GETSIZE(ptr - SIZE));
	pthread_mutex_unlock(&mutex_malloc);
}
