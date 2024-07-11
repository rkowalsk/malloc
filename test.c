#include <stdio.h>
#include "malloc.h"
#include <sys/mman.h>

int main(void)
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
}

/*int main(void)
{
	void *a = ft_malloc(666);
	(void) a;
	return (0);
}*/
