#include "../ft_malloc.h"

void hexdump(const void* data, size_t size) {
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
