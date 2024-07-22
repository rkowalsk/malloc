#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
#include <errno.h>
#include <sys/mman.h>

/*int main(void)
{
	void *a = mmap(NULL, 0x3000, PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	printf("%p\n", a);
	*(int *)a = 1337;
	*(int *)(a + 0x1000) = 1427;
	*(int *)(a + 0x2000) = 1995;
	printf("%d %d %d\n", *(int *)a, *(int *)(a + 0x1000), *(int *)(a + 0x2000));
	munmap(a + 0x1000, 0x1000);
	printf("%d\n", *(int *)a);
	printf("%d\n", *(int *)(a + 0x2000));
	printf("%d\n", *(int *)(a + 0x1000));
	return(0);
}*/

/*int main(void)
{
	printf("%ld\n", new_heap_size(0));
	return (0);
}*/

/*
int main(void)
{
	char *truc = malloc(0);
	dprintf(1, "%lu\n", (* (unsigned long *) (truc - 8)) & 0xFFFFFFFFFFFFFFF8);
	dprintf(1, "%p\n", truc);
	int i = 0;
	while (1)
	{
		truc[i] = 'A';
		dprintf(1, "%d: %s\n", i, (char *) truc);
		i++;
	}
	return (0);
}
*/
int main(void)
{
	void *a;
	int i = 2;
	while (i < 10000)
	{
		a = ft_malloc(i);
		if (!a)
		{
			perror("malloc");
			return (2);
		}
		dprintf(1, "address = %p | aligned ? %d | i = %5d | size = %lu\n", a, ((long) a % 16) == 0, i, (* (unsigned long *) (a - 8)) & 0xFFFFFFFFFFFFFFF8);
		((char *) a)[0] = 'A';
		((char *) a)[i - 1] = 'A';
		i += 2;
	}
	return (0);
}

/*
int main(void)
{
	void	*a;
	unsigned long	*val1;
	unsigned long	*val2;
	unsigned long	*val3;
	struct unused_chunk *b;
	dprintf(1, "size long: %lu\n", sizeof(unsigned long));
	dprintf(1, "size struct : %lu\n", sizeof(struct unused_chunk));
	a = ft_malloc(1);
	val1 = a - 16;
	val2 = a - 8;
	val3 = a;
	b = a - 16;
//	dprintf(1, "%p %lu %lu\n", a, * (long *) (a - 8), * (long *) (a - 16));
	dprintf(1, "%p\n", a);
	dprintf(1, "0x%lx 0x%lx 0x%lx\n", *val1, *val2, *val3);
	dprintf(1, "0x%lx", b->size);
	print_chunk(b);
	return (0);
}

int	main(void)
{
	struct unused_chunk *a;
	a = ft_malloc(1);
	a -= 16;
	print_chunk(a);
	dprintf(1, "sizeof struct: %ld\n", sizeof(*a));
	dprintf(1, "a: %p, prev_size: %p, size: %p, fwd: %p, bwd: %p\n", a, &(a->prev_size), &(a->size), &(a->fwd), &(a->bwd));
	dprintf(1, "size: %lu\n", * (unsigned long *) &(a->size));
	dprintf(1, "size: %lu\n", * (unsigned long *) (a + 8));
	dprintf(1, "size: %lx\n", * (unsigned long *) &(a->size));
	dprintf(1, "size: %lx\n", * (unsigned long *) (a + 8));
	dprintf(1, "size: %lu\n", * (unsigned long *) (a + ((struct unused_chunk *) &(a->size) - a)));
	dprintf(1, "prev_size: %lu\n", * (unsigned long *) (a + ((struct unused_chunk *) &(a->prev_size) - a)));
	dprintf(1, "%lu\n", ((struct unused_chunk *) &(a->size) - a));
	dprintf(1, "%lu\n", ((struct unused_chunk *) &(a->prev_size) - a));
	return (0);
}
*/
