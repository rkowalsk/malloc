#include "ft_malloc.h"
#include <stdio.h>
// #include <stdlib

void cspam(char *start, char c, int size)
{
	int i = 0;
	while (i < size)
	{
		start[i] = c;
		i++;
	}
}

void	print_alloc(char *a, size_t i)
{
	struct unused_chunk *b;
	dprintf(1, "malloc size = %ld", i);
	if (!a)
	{
		perror("malloc");
		exit (2);
	}
	b = (void *) (a - 16);
	dprintf(1, " | address malloc = %p | address struct = %p | aligned ? %d | size = %lu", a, b, ((unsigned long)a % 16) == 0, b->size & 0xFFFFFFFFFFFFFFF8);
	cspam(a, 'A', i - 1);
	a[i - 1] = 0;
	dprintf(1, " | stored string size = %ld\n", strlen(a));
}

int main(void)
{
	char	*a;
	char	*b;
	int		size = 1024;

	a = ft_malloc(size);
	print_alloc(a, size);
	show_alloc_mem();
	b = ft_malloc(size);
	print_alloc(b, size);
	show_alloc_mem();
	print_list(lists.free);
	return (0);
}
