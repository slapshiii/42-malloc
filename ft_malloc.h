
#ifndef FT_MALLOC_H
	#define FT_MALLOC_H
	#define _GNU_SOURCE

	#include <sys/mman.h>
	//#include <stdio.h>
	#include <unistd.h>
	#include <pthread.h>
	#include <libft.h>
	#include <errno.h>

	//DEBUG
	#include <fcntl.h>
	#include <signal.h>

	/**
	 * 	for PAGESIZE 4096
	 *  small: < 256
	 *  medium: >= 256 && < 2048
	 *  large: > 2048
	 */
	#define PAGESIZE getpagesize()
	#define TINY_ZONE_SIZE (size_t)(8 * PAGESIZE)
	#define TINY_CHUNK_SIZE (size_t)(TINY_ZONE_SIZE / 128)
	#define SMALL_ZONE_SIZE (size_t)(64 * PAGESIZE)
	#define SMALL_CHUNK_SIZE (size_t)(SMALL_ZONE_SIZE / 128)

	#ifndef INTERNAL_SIZE_T
	# define INTERNAL_SIZE_T size_t
	#endif

	#if !defined(MAP_ANONYMOUS) && defined(MAP_ANON)
	# define MAP_ANONYMOUS MAP_ANON
	#endif

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

	typedef struct	malloc_heap {
		INTERNAL_SIZE_T		chk_cnt;
		INTERNAL_SIZE_T		size;
		struct malloc_heap*	fd;
		struct malloc_heap*	bk;
	}				heap_t;

	typedef struct	malloc_chunk {
		INTERNAL_SIZE_T			prev_size;
		INTERNAL_SIZE_T			size;
		struct malloc_chunk*	fd;
		struct malloc_chunk*	bk;
	}				chunk_t;

	typedef struct	victim_info {
		heap_t			*heap;
		chunk_t			*chunk;
		INTERNAL_SIZE_T	size;
	}				victim_info_t;

	#define SIZE_SZ				(sizeof (INTERNAL_SIZE_T))
	#define MALLOC_ALIGNMENT	(2 * SIZE_SZ)
	#define MALLOC_ALIGN_MASK	(MALLOC_ALIGNMENT - 1)
	#define MIN_CHUNK_SIZE		(sizeof (struct malloc_chunk))
	#define MINSIZE				(sizeof (struct malloc_chunk))

	#define PREV_INUSE		0x1
	#define IS_MMAPPED		0x2
	#define NON_MAIN_ARENA	0x4
	
	#define ISALLOC(p)	((p)->size & PREV_INUSE)
	#define ISMMAP(p)	((p)->size & IS_MMAPPED)
	#define ISMAIN(p)	((p)->size & NON_MAIN_ARENA)
	#define CLEAR_INUSE(p)	((p)->size &= ~(PREV_INUSE))

	#define SIZE_BITS			(PREV_INUSE | IS_MMAPPED | NON_MAIN_ARENA)
	#define GETSIZE_NOMASK(p)	((p)->size)
	#define GETSIZE(p)			(GETSIZE_NOMASK(p) & ~(SIZE_BITS))

	#define next_chunk(p)	((chunk_t*)(((char *)(p)) + GETSIZE(p)))
	#define prev_chunk(p)	((chunk_t*)(((char *)(p)) - (p)->prev_size))

	#define chunk2mem(p)	((void*)((void*)(p) + 2*SIZE_SZ))
	#define mem2chunk(mem)	((chunk_t*)((void*)(mem) - 2*SIZE_SZ))
	#define heap2chunk(h)	((chunk_t*)((void*)(h) + sizeof(heap_t)))
	#define chunk2heap(p)	((heap_t*)((void*)(p) - sizeof(heap_t)))

	enum lst_size {
		e_tiny, e_small, e_large
	};

	typedef struct	malloc_s {
		heap_t			*heaplist[3];
		size_t			pagesize;
		debug_malloc_t	debug;
	}				malloc_t;

	extern malloc_t			m;
	extern pthread_mutex_t	mutex_malloc;

	void	ft_free(void *ptr);
	void	*ft_malloc(size_t size);
	void	*ft_realloc(void *ptr, size_t size);
	void	show_alloc_mem();

	void    show_alloc_mem_hex(void *ptr);

	void	intern_free(void *ptr);
	void	*intern_malloc(size_t size);
	void	*intern_realloc(void *ptr, size_t size);

	heap_t	*create_heap(size_t s, heap_t *prev_heap);
	heap_t	*get_last_heap(heap_t *root);
	chunk_t	*get_last_heap_block(heap_t *root);
	void	*allocate(heap_t** l, size_t s);

	victim_info_t get_ptr_info(void *ptr);

	void	init_chunk(chunk_t *addr, size_t size, int free, size_t prev_sz);

	void	merge_next_chunk(chunk_t *chunk);

	victim_info_t	get_first_fit(heap_t* l, size_t s);
	size_t	get_zone_size(size_t size);
	size_t	get_align_size(const size_t size);
	size_t	print_bucket(heap_t *root);
	void	hexdump(const void* data, size_t size);

	void	ft_putptr_fd(void *p, int fd);
	void	ft_puthex_char(unsigned char c, int fd);

	// DEBUG
	// void	report_allocations_option(const char *option);
	// void	validate_ptrs_option(const char *option);
	// void	output_option(const char *option);
	// void	pattern_alloc_option(const char *option);
	// void	pattern_free_option(const char *option);

	// void	report_allocations(void);
	// void	abort_validate_ptr(void *ptr);
	// int		validate_ptr(void *ptr);
	// void	fill_pattern(void *addr, char *pattern, size_t size);

#endif	//FT_MALLOC_H