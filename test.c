#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>

int main(void)
{
//	void *a = mmap(NULL, 1, PROT_READ | PROT_WRITE,
//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	void *a = malloc(144);
	printf("%p\n", a);
	return(0);
}
