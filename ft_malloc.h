
#ifndef FT_MALLOC_H
#define FT_MALLOC_H

#define _GNU_SOURCE 1	//to grab REG_RIP (debug)

#include <sys/mman.h>
//#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <libft.h>

//DEBUG
#include <fcntl.h>
#include <signal.h>

/**											Zonesize minimum
 *  small: < pagesize/8 					---> 0xD000			0x10000 (128 * pagesize/8)
 *  medium: >= pagesize/8 && < pagesize		---> 0x64000		0x80000 (128 * pagesize)
 *  large: > pagesize
 */
#define SMALLZONE 0xD000
#define MEDIUMZONE 0x64000

#if defined(__x86_64__)
/* 64 bit detected */
	#define SIZE 8UL
	#define MALLOC_ALIGNMENT 8UL
	#define MINSIZE 16UL
	#define MAXSIZE (m.pagesize - 4 * SIZE)
#endif
#if defined(__i386__)
/* 32 bit x86 detected */
	#define SIZE 8UL
	#define MALLOC_ALIGNMENT 8UL
	#define MINSIZE 16UL
	#define MAXSIZE (m.pagesize - 4 * SIZE)
#endif

#define ISALLOC(a) (*(size_t*)(a) & 0b001)
#define ISMMAP(a) (*(size_t*)(a) & 0b010)
#define ISMAIN(a) (*(size_t*)(a) & 0b100)

#define GETSIZE(a) (*(size_t*)(a) & (~0b111))

#define BKPTR (2 * SIZE)
#define FDPTR (SIZE)

typedef enum	valid_ptrs_val {
	E_OFF = -1, E_ON, E_ALLOCATED, E_FREED, E_INVALID
}				ptrs_val_t;

typedef struct	debug_malloc_s {
	int				b_debug;
	int				report_allocations;
	ptrs_val_t		validate_ptrs;
	int				output;
	char			pattern_alloc[128];
	char			pattern_free[128];
}				debug_malloc_t;

typedef struct	malloc_s {
	void*			lst_page_s;
	void*			lst_page_m;
	void*			lst_page_l;
	void*			lst_free_s;
	void*			lst_free_m;
	size_t			pagesize;
	debug_malloc_t	debug;
}				malloc_t;

malloc_t		m;
pthread_mutex_t	mutex_malloc;

void	free(void *ptr);
void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	show_alloc_mem();
void    show_alloc_mem_hex(void *ptr);

void	*allocate(void** l, void **fl, size_t s);
void	*allocate_large(void** l, size_t s);
void	desallocate(void *ptr, void **fl, size_t size);
void	desallocate_large(void *ptr, size_t size);

void	*format_chunk_a(void *addr, void **fl, size_t s);
void	*format_chunk_f(void *addr, void **fl, size_t s);
void	*free_merge_contiguous(void *ptr);
int		try_extend_chunk(void *ptr, size_t size);
void	set_value(void *addr, size_t val);
size_t	get_value(void *addr);

void	*get_first_fit(void* l, size_t s);
void	*get_last_free(void** l);
size_t	get_max_zone(size_t size);
size_t	print_bucket(void *root);
void	hexdump(const void* data, size_t size);

void	ft_putptr_fd(void *p, int fd);
void	ft_puthex_char(unsigned char c, int fd);

// DEBUG
void	report_allocations_option(const char *option);
void	validate_ptrs_option(const char *option);
void	output_option(const char *option);
void	pattern_alloc_option(const char *option);
void	pattern_free_option(const char *option);

void	report_allocations(void);
void	abort_validate_ptr(void *ptr);
int		validate_ptr(void *root, void *ptr);
void	fill_pattern(void *addr, char *pattern, size_t size);

#endif