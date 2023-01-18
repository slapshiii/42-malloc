#include "../ft_malloc.h"

malloc_t m = {
	.lst_page_s = NULL,
	.lst_page_m = NULL,
	.lst_page_l = NULL,
	.lst_free_s = NULL,
	.lst_free_m = NULL
};

pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;

__attribute__((constructor))
static void initialize_malloc() {
	char * pagesize_str = getenv("PAGESIZE");
	m.pagesize = (pagesize_str == NULL) ?
		(size_t)getpagesize() : (size_t)ft_atoi(pagesize_str);
	char * malloctype = getenv("MALLOCTYPE");
	if (malloctype && ft_strcmp(malloctype, "debug") == 0) {
		m.debug.b_debug = 1;
		char * mallocdebug = getenv("MALLOCDEBUG");
		redirect_output(mallocdebug);
	}
}

__attribute__((destructor))
static void destroy_malloc() {
	if (m.debug.b_debug) {
		if (m.debug.output > 2)
			close(m.debug.output);
	}
}

void    free(void *ptr) {
	pthread_mutex_lock(&mutex_malloc);
	size_t size = GETSIZE(ptr - SIZE);
	if (size < m.pagesize/4) {
		desallocate(ptr - SIZE, &m.lst_free_s, size);
	}
	else if (size < m.pagesize) {
		desallocate(ptr - SIZE, &m.lst_free_m, size);
	}
	else {
		desallocate_large(ptr - SIZE, size);
	}
	pthread_mutex_unlock(&mutex_malloc);
}

void    *malloc(size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	void    *res = NULL;

	if (size < m.pagesize/4) {
		res = allocate(&m.lst_page_s, &m.lst_free_s, size);
	}
	else if (size < m.pagesize) {
		res = allocate(&m.lst_page_m, &m.lst_free_m, size);
	}
	else {
		res = allocate_large(&m.lst_page_l, size);
	}
	pthread_mutex_unlock(&mutex_malloc);
	return (res);
}

void    *realloc(void *ptr, size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	if (try_extend_chunk(ptr, size)) {
		void *res = malloc(size);
		if (res != NULL) {
			size = (GETSIZE(ptr - SIZE) < size) ? GETSIZE(ptr - SIZE) : size;
			ft_memmove(res, ptr, size);
			free(ptr);
		}
		ptr = res;
	}
	pthread_mutex_unlock(&mutex_malloc);
	return (ptr);
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

