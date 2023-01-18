#include "../ft_malloc.h"

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
    unsigned char hi = (c >> 4) & 0xf; 
    unsigned char lo = c & 0xf;        
    char tmp[2] = {hi, lo};

    tmp[0] += hi < 10 ? '0' : 'A' - 10;
    tmp[1] += lo < 10 ? '0' : 'A' - 10;

    write(fd, tmp, 2);
}