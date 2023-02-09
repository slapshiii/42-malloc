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

void	print_chunk(chunk_t *chunk) {
	ft_putptr_fd(chunk, m.debug.output);
	ft_putstr_fd(" - ", m.debug.output);
	ft_putptr_fd(chunk + GETSIZE(chunk) + SIZE_SZ - 1, m.debug.output);
	ft_putstr_fd(" | ", m.debug.output);
	ft_putnbr_fd((int)GETSIZE(chunk), m.debug.output);
	ft_putstr_fd(" bytes", m.debug.output);
	if (ISALLOC(chunk)) {
		ft_putstr_fd(" allocated\n", m.debug.output);
	} else {
		ft_putstr_fd(" free\n", m.debug.output);
	}
}

size_t	print_bucket(heap_t *root) {
	size_t res = 0;
	while (root != NULL)
    {
		if (ISMMAP(root)) {
			print_chunk((chunk_t *)root);
			if (ISALLOC(root))
				res += GETSIZE(root);
		} else {
			chunk_t *chunk = heap2chunk(root);
			for (INTERNAL_SIZE_T i = 0; i < root->chk_cnt; ++i) {
				print_chunk(chunk);
				if (ISALLOC(chunk))
					res += GETSIZE(chunk);
				chunk = next_chunk(chunk);
			}
		}
		root = root->fd;
    }
	return (res);
}

void    show_alloc_mem() {
	pthread_mutex_lock(&mutex_malloc);
	size_t total = 0;
	ft_putendl_fd("TINY : ", m.debug.output);
	total += print_bucket(m.heaplist[e_tiny]);
	ft_putendl_fd("SMALL : ", m.debug.output);
	total += print_bucket(m.heaplist[e_small]);
	ft_putendl_fd("LARGE : ", m.debug.output);
	total += print_bucket(m.heaplist[e_large]);
	ft_putstr_fd("Total : ", m.debug.output);
	ft_putnbr_fd(total, m.debug.output);
	ft_putstr_fd(" bytes\n", m.debug.output);
	pthread_mutex_unlock(&mutex_malloc);
}

void    show_alloc_mem_hex(void *ptr) {
	pthread_mutex_lock(&mutex_malloc);
	victim_info_t victim = get_ptr_info(ptr);
	if (check_ptr(victim, ptr))
		return;
	if (ISMMAP(victim.heap))
		hexdump(ptr, GETSIZE(victim.heap) - sizeof(heap_t));
	else
		hexdump(ptr, GETSIZE(victim.chunk) - 2*SIZE_SZ);
	pthread_mutex_unlock(&mutex_malloc);
}
