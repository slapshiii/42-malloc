#include "../ft_malloc.h"

malloc_t m = {
	.heaplist = {NULL, NULL, NULL}
};

pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;