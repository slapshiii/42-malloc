#include "../ft_malloc.h"

void	redirect_output(const char *option) {
	char *start = ft_strnstr(option, "output:", ft_strlen(option));
	if (start != NULL) {
		start += ft_strlen("output:");
		char *end = ft_strchr(start, ',');
		if (end == NULL)
			end = start + ft_strlen(start);
		char buf[end-start + 1];
		ft_strlcpy(buf, start, end - start + 1);
		ft_putendl_fd(buf, 1);
		if (ft_strcmp(buf, "stderr") == 0)
			m.debug.output = 2;
		else if (ft_strcmp(buf, "stdout") == 0)
			m.debug.output = 1;
		else
			m.debug.output = open(buf, O_CREAT|O_WRONLY|O_TRUNC);
	} else
		m.debug.output = 2;
}