#include "../ft_malloc.h"

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
	} else {
		m.debug.b_debug = 0;
		m.debug.validate_ptrs = E_OFF;
	}
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

void	report_allocations_option(const char *option) {
	char *start = ft_strnstr(option, "report_allocations", ft_strlen(option));
	if (start != NULL) {
		m.debug.report_allocations = 1;
	} else
		m.debug.report_allocations = 0;
}

void	validate_ptrs_option(const char *option) {
	char *start = ft_strnstr(option, "validate_ptrs", ft_strlen(option));
	if (start != NULL) {
		m.debug.validate_ptrs = E_ON;
	} else
		m.debug.validate_ptrs = E_OFF;
}

void	output_option(const char *option) {
	char *start = ft_strnstr(option, "output:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("output:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		char buf[end-start + 1];
		ft_strlcpy(buf, start, end - start + 1);
		if (ft_strcmp(buf, "stderr") == 0)
			m.debug.output = 2;
		else if (ft_strcmp(buf, "stdout") == 0)
			m.debug.output = 1;
		else
			m.debug.output = open(buf, O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU|S_IROTH);
	} else
		m.debug.output = 2;
}

void	pattern_alloc_option(const char *option) {
	char *start = ft_strnstr(option, "fill:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("fill:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		ft_strlcpy(m.debug.pattern_alloc, start, end - start + 1);
	} else
		bzero(m.debug.pattern_alloc, 128);
}

void	pattern_free_option(const char *option) {
	char *start = ft_strnstr(option, "fill_free:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("fill_free:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		ft_strlcpy(m.debug.pattern_free, start, end - start + 1);
	} else
		bzero(m.debug.pattern_free, 128);
}
