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
	m.pagesize &= ~(0x0FFF);
	char * mallocdebug = getenv("MALLOCDEBUG");
	if (mallocdebug) {
		m.debug.b_debug = 1;
		report_allocations_option(mallocdebug);
		validate_ptrs_option(mallocdebug);
		output_option(mallocdebug);
		pattern_alloc_option(mallocdebug);
		pattern_free_option(mallocdebug);
	} else
		m.debug.b_debug = 0;
}

__attribute__((destructor))
static void destroy_malloc() {
	if (m.debug.b_debug) {
		if (m.debug.report_allocations)
			report_allocations();
		if (m.debug.output > 2)
			close(m.debug.output);
	}
}

void    free(void *ptr) {
	pthread_mutex_lock(&mutex_malloc);
	size_t size = GETSIZE(ptr - SIZE);
	if (size < m.pagesize/4) {
		if (
		m.debug.validate_ptrs != E_OFF
		&& validate_ptr(m.lst_page_s, ptr - SIZE) != E_ALLOCATED
		)
			abort_validate_ptr(ptr);
		desallocate(ptr - SIZE, &m.lst_free_s, size);
	}
	else if (size <= MAXSIZE) {
		if (
		m.debug.validate_ptrs != E_OFF
		&& validate_ptr(m.lst_page_m, ptr - SIZE) != E_ALLOCATED
		)
			abort_validate_ptr(ptr);
		desallocate(ptr - SIZE, &m.lst_free_m, size);
	}
	else {
		if (
		m.debug.validate_ptrs != E_OFF
		&& validate_ptr(m.lst_page_l, ptr - SIZE) != E_ALLOCATED
		)
			abort_validate_ptr(ptr);
		desallocate_large(ptr - SIZE, size);
	}
	pthread_mutex_unlock(&mutex_malloc);
}

void    *malloc(size_t size) {
	pthread_mutex_lock(&mutex_malloc);
	void    *res = NULL;
	if (size < MINSIZE)
		size = MINSIZE;
	else if (size % SIZE)
		size = (size + SIZE) & ~(SIZE - 1);
	if (size < m.pagesize/4) {
		res = allocate(&m.lst_page_s, &m.lst_free_s, size);
	}
	else if (size <= MAXSIZE) {
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
	if (m.debug.validate_ptrs != E_OFF && (
		validate_ptr(m.lst_page_s, ptr - SIZE) != E_ALLOCATED &&
		validate_ptr(m.lst_page_m, ptr - SIZE) != E_ALLOCATED &&
		validate_ptr(m.lst_page_l, ptr - SIZE) != E_ALLOCATED
	))
		abort_validate_ptr(ptr);
	if (try_extend_chunk(ptr, size)) {
		pthread_mutex_unlock(&mutex_malloc);
		void *res = malloc(size);
		pthread_mutex_lock(&mutex_malloc);
		if (res != NULL) {
			ft_memmove(res, ptr, GETSIZE(ptr - SIZE));
			pthread_mutex_unlock(&mutex_malloc);
			free(ptr);
			pthread_mutex_lock(&mutex_malloc);
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

void    show_alloc_mem_hex(void *ptr) {
	if (m.debug.validate_ptrs != E_OFF && (
		validate_ptr(m.lst_page_s, ptr - SIZE) == E_INVALID &&
		validate_ptr(m.lst_page_m, ptr - SIZE) == E_INVALID &&
		validate_ptr(m.lst_page_l, ptr - SIZE) == E_INVALID
	))
		abort_validate_ptr(ptr);
	pthread_mutex_lock(&mutex_malloc);
	hexdump(ptr, GETSIZE(ptr - SIZE));
	pthread_mutex_unlock(&mutex_malloc);
}

