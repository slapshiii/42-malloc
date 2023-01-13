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
	char ascii[9];
	size_t i, j;
	ascii[8] = '\0';
	for (i = 0; i < size; ++i) {
        if (i % 8 == 0)
            printf("%p  |  ", data + i);
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 8] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 8] = '.';
		}
		if ((i+1) % 4 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 8 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 8] = '\0';
				if ((i+1) % 8 <= 4) {
					printf(" ");
				}
				for (j = (i+1) % 8; j < 8; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}

size_t	print_bucket(void *root) {
	size_t res = 0;
	while (root != NULL && GETSIZE(root) != 0)
    {
        printf("%lX - %lX | %ld bytes",
                (unsigned long)root, (unsigned long)(root + GETSIZE(root)), GETSIZE(root));
        if (ISALLOC(root)) {
			res += GETSIZE(root);
            printf(" allocated\n");
        } else {
            printf(" free\n");
        }
        root=(size_t*)(root + GETSIZE(root) + 2 * SIZE);
		if (get_value(root) == 0b01)
			root = (void *)get_value(root + FDPTR);
    }
	return (res);
}
