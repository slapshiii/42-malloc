#include "../ft_malloc.h"

malloc_t m = {
	.lst_page_s = NULL,
	.lst_page_m = NULL,
	.lst_page_l = NULL,
	.lst_free_s = NULL,
	.lst_free_m = NULL
};

pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;