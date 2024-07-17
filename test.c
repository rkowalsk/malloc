#include <stdio.h>
#include <unistd.h>
#include <malloc.h>
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

long	new_heap_size(size_t needed_size)
{
	long	page_size;
	long	heap_size;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (-1);
	needed_size += 8;
	heap_size = needed_size / page_size;
	heap_size = (heap_size + ((needed_size % page_size) > 0)) * page_size;
	return (heap_size);
}

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
	int i = 0;
	while (i < 10000)
	{
		a = malloc(i);
		dprintf(1, "i = %5d size = %lu\n", i, (* (unsigned long *) (a - 8)) & 0xFFFFFFFFFFFFFFF8);
		i += 16;
	}
	return (0);
}
/*
int main(void)
{
	printf("%ld\n", sizeof(long));
	return (0);
}
*/
