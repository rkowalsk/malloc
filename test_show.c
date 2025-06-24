#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include <sys/mman.h>
#include "ft_malloc.h"

void cspam(char *start, char c, int size)
{
	int i = 0;
	while (i < size)
	{
		start[i] = c;
		i++;
	}
}

int main(void)
{
	char *a, *b, *c, *d;
	// char *a, *b, *c;
	a = ft_malloc(10);
	if (!a)
	{
		perror("malloc");
		return (2);
	}
	cspam(a, 'A', 10);
	b = ft_malloc(100);
	if (!b)
	{
		perror("malloc");
		return (2);
	}
	cspam(b, 'B', 100);
	c = ft_malloc(1000);
	if (!c)
	{
		perror("malloc");
		return (2);
	}
	cspam(c, 'C', 1000);
	d = ft_malloc(100000);
	if (!d)
	{
		perror("malloc");
		return (2);
	}
	cspam(d, 'D', 100000);
	dprintf(1, "amogus\n");
	show_alloc_mem_ex();
	ft_free(a);
	ft_free(b);
	ft_free(c);
	ft_free(d);
	show_alloc_mem();
}
