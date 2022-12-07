ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME :=	libft_malloc_$(HOSTTYPE).so
SRCDIR := src
OBJDIR := obj

C_FILE = malloc.c

SRCS =	$(addprefix $(SRCDIR)/, $(C_FILE))
OBJS = 	$(addprefix $(OBJDIR)/, $(notdir $(SRCS:%.c=%.o)))

CFLAGS = -Wall -Werror -Wextra
CFLAGS += -fPIC

all:	${NAME}

${NAME}:	${OBJS}
	gcc ${OBJS} -shared -o ${NAME}

$(OBJDIR)/%.o:	$(SRCDIR)/%.c ft_malloc.h
	@mkdir -p $(OBJDIR)
	gcc ${CFLAGS} -c $< -o $@ -I./

test: ${NAME}
	gcc main.c ${NAME} -o a.out -I./
	@./a.out
	@rm a.out

clean:
	rm -rf ${OBJDIR}

fclean:		clean
	rm -rf ${NAME}

re:		fclean all


.PHONY:		all clean fclean re test
