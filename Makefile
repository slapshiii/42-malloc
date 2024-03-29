ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME :=	libft_malloc_$(HOSTTYPE).so
SRCDIR := src
OBJDIR := obj
LIBFTDIR := libft

SUBDIR := libft
INC_DIRS = -I./ $(addprefix -I, $(SUBDIR))
LIBS = -lpthread -lft
LIB_DIRS = -L./ -L./libft

C_FILE =	malloc.c \
			free.c \
			realloc.c \
			globals.c \
			heap.c \
			chunk.c \
			malloc_utils.c \
			printer.c \
			bonus.c \
			malloc_debug.c

INCS := ft_malloc.h libft/libft.h

SRCS =	$(addprefix $(SRCDIR)/, $(C_FILE))
OBJS = 	$(addprefix $(OBJDIR)/, $(C_FILE:%.c=%.o))

CFLAGS += -Wall -Werror -Wextra -g3

CC=gcc

all:	${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -shared -o ${NAME} -I./${LIBFTDIR} ${OBJS} $(LIB_DIRS) $(LIBS)
	-ln -sf ${NAME} libft_malloc.so

$(OBJDIR)/%.o:	$(SRCDIR)/%.c $(INCS) ${LIBFTDIR}/libft.a
	@mkdir -p $(OBJDIR)
	${CC} ${CFLAGS} -fPIC $(INC_DIRS) -c $< -o $@ $(LIB_DIRS) $(LIBS)

${LIBFTDIR}/libft.a:
	$(MAKE) -C $(LIBFTDIR)/ all

test: ${NAME}
	${CC} $(INC_DIRS) main.c -L. -lft_malloc
	-/usr/bin/time -v ./run.sh ./a.out

clean:
	-rm -rf ${OBJDIR}
	$(MAKE) -C $(LIBFTDIR)/ clean

fclean:		clean
	-rm -rf ${NAME} a.out *.txt
	-unlink libft_malloc.so
	$(MAKE) -C $(LIBFTDIR)/ fclean

re:		fclean all

.PHONY:		all clean fclean re test
