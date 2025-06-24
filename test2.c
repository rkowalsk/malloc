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
	int i = 10000;
	char *a;
	struct unused_chunk *b;
	while (i > 1)
	{
		a = malloc(i);
		dprintf(1, "malloc size = %d", i);
		if (!a)
		{
			perror("malloc");
			return (2);
		}
		b = (void *) (a - 16);
		dprintf(1, " | address malloc = %p | address struct = %p | aligned ? %d | size = %lu", a, b, ((unsigned long)a % 16) == 0, b->size & 0xFFFFFFFFFFFFFFF8);
		cspam(a, 'A', i - 1);
		a[i - 1] = 0;
		dprintf(1, " | stored string size = %ld\n", strlen(a));
		free(a);
		i -= 1;
	}
	print_list(lists.free);
	return (0);
}
