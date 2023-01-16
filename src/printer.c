#include "../ft_malloc.h"

static size_t	ft_strlen(const char *s)
{
	size_t	len;

	len = 0;
	while (s[len])
		len++;
	return (len);
}

void    ft_putptr_fd(void *p, int fd) {
    unsigned char buf[sizeof(void *)];
    ft_memmove(buf, &p, sizeof(void *));
    write(fd, "0x", 2);
    for (int i = sizeof(void *) - 1; i >= 0; i--) {
        unsigned char hi = (buf[i] >> 4) & 0xf; 
        unsigned char lo = buf[i] & 0xf;        
        char tmp[2] = {hi, lo};

        tmp[0] += hi < 10 ? '0' : 'A' - 10;
        tmp[1] += lo < 10 ? '0' : 'A' - 10;

        write(fd, tmp, 2);
    }
}

void    ft_puthex_char(unsigned char c, int fd) {
    unsigned char buf[sizeof(unsigned char)];
    ft_memmove(buf, &c, sizeof(unsigned char));
    for (int i = sizeof(unsigned char) - 1; i >= 0; i--) {
        unsigned char hi = (buf[i] >> 4) & 0xf; 
        unsigned char lo = buf[i] & 0xf;        
        char tmp[2] = {hi, lo};

        tmp[0] += hi < 10 ? '0' : 'A' - 10;
        tmp[1] += lo < 10 ? '0' : 'A' - 10;

        write(fd, tmp, 2);
    }
}

static void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

void	ft_putnbr_fd(int nb, int fd)
{
	if (nb == -2147483648)
	{
		ft_putchar_fd('-', fd);
		ft_putchar_fd('2', fd);
		ft_putnbr_fd(147483648, fd);
	}
	else if (nb < 0)
	{
		ft_putchar_fd('-', fd);
		ft_putnbr_fd(-nb, fd);
	}
	else if (nb > 9)
	{
		ft_putnbr_fd(nb / 10, fd);
		ft_putnbr_fd(nb % 10, fd);
	}
	else
		ft_putchar_fd(nb + '0', fd);
}

void	ft_putstr_fd(char *s, int fd)
{
	if (fd >= 0 && s)
		write(fd, s, ft_strlen(s));
}