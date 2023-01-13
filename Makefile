ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

export LD_LIBRARY_PATH := $(PWD)/:$(LD_LIBRARY_PATH)
NAME :=	libft_malloc_$(HOSTTYPE).so
SRCDIR := src
OBJDIR := obj
LIBFTDIR := libft

C_FILE =	malloc.c \
			malloc_page.c \
			malloc_mem.c \
			malloc_utils.c

SRCS =	$(addprefix $(SRCDIR)/, $(C_FILE))
OBJS = 	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CFLAGS += -Wall -Werror -Wextra -g3
CFLAGS += -fPIC

#CC=LD_LIBRARY_PATH=$(PWD) gcc
CC=gcc

all:	${NAME}

${NAME}: libft.a ${OBJS}
	${CC} ${CFLAGS} -shared -o ${NAME} ${OBJS} -L$(LIBFTDIR) -lft
	-ln -sf ${NAME} libft_malloc.so

$(OBJDIR)/%.o:	$(SRCDIR)/%.c ft_malloc.h
	@mkdir -p $(OBJDIR)
	${CC} ${CFLAGS} -c $< -o $@ -L$(LIBFTDIR) -lft -I./$(LIBFTDIR)

libft.a:
	$(MAKE) -C $(LIBFTDIR)/

test: ${NAME}
	${CC} -I. -o a.out main.c -L. -lft_malloc
	@-./a.out > test.txt

clean:
	-rm -rf ${OBJDIR}
	$(MAKE) -C $(LIBFTDIR)/ clean

fclean:		clean
	-rm -rf ${NAME} a.out
	-unlink libft_malloc.so
	$(MAKE) -C $(LIBFTDIR)/ fclean

re:		fclean all

.PHONY:		all clean fclean re test
