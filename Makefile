ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

export LD_LIBRARY_PATH := $(PWD)/:$(LD_LIBRARY_PATH)
NAME :=	libft_malloc_$(HOSTTYPE).so
SRCDIR := src
OBJDIR := obj
LIBFTDIR := libft

SUBDIR := libft
INC_DIRS = -I./ $(addprefix -I, $(SUBDIR))
LIBS = -lpthread -lft
LIB_DIRS = -L./ -L./libft

C_FILE =	malloc.c \
			malloc_page.c \
			malloc_mem.c \
			malloc_utils.c \
			printer.c

SRCS =	$(addprefix $(SRCDIR)/, $(C_FILE))
OBJS = 	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CFLAGS += -Wall -Werror -Wextra -g3
CFLAGS += -fPIC
# CFLAGS += fsanitize=address

#CC=LD_LIBRARY_PATH=$(PWD) gcc
CC=gcc

all:	${NAME}

${NAME}: ${OBJS}
	${CC} ${CFLAGS} -shared -o ${NAME} -I./${LIBFTDIR} ${OBJS} $(LIB_DIRS) $(LIBS)
	-ln -sf ${NAME} libft_malloc.so

$(OBJDIR)/%.o:	$(SRCDIR)/%.c $(INCS) libft.a
	@mkdir -p $(OBJDIR)
	${CC} ${CFLAGS} $(INC_DIRS) -c $< -o $@ $(LIB_DIRS) $(LIBS)

libft.a:
	$(MAKE) -C $(LIBFTDIR)/ all

test: ${NAME}
	${CC} $(INC_DIRS) -o a.out main.c -L. -lft_malloc $(LIB_DIRS) $(LIBS)
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
