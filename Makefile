ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME = libft_malloc_$(HOSTTYPE).so

GENERIC = libft_malloc.so

SRCS = srcs/malloc.c \
	   srcs/free.c \
	   srcs/init.c \
	   srcs/prealloc.c \
	   srcs/free_list.c \
	   srcs/heap_list.c \
	   srcs/utils.c

OBJS = ${SRCS:.c=.o}

LIBFT_FILE = libft/libft.a

HEADERS =	includes/libft.h \
			includes/ft_malloc.h

HEADERS_DIR = includes

$(NAME): $(LIBFT_FILE) $(OBJS) $(HEADERS)
	gcc -g -shared $(OBJS) $(LIBFT_FILE) -o $@
	ln -sf $@ $(GENERIC)

$(OBJS): %.o: %.c $(HEADERS)
	gcc -Wall -Wextra -Werror -Werror=pointer-arith -g -fPIC -I$(HEADERS_DIR) -c $< -o $@

$(LIBFT_FILE):
	make -C libft

.PHONY = all clean fclean re

all: $(NAME)

clean:
	rm -f $(OBJS)
	make -C libft clean

fclean: clean
	rm -f $(NAME) $(GENERIC) $(LIBFT_FILE)

re: fclean $(NAME)
