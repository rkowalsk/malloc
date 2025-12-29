#include "ft_malloc.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);

int	main(void)
{
	void	*ptr;
	size_t	size;
	int		i;
	int		do_free;
	struct unused_chunk	*chunk;

	srand(420);
	ptr = NULL;
	i = 0;

	while (i < 1000000)
	{
		size = (rand() % 100000) + 1;
		ptr = ft_malloc(size);
		if (!ptr)
		{
			printf("i=%d size=%zu malloc=FAIL\n", i, size);
			return (1);
		}
		memset(ptr, 0xAB, size);
		chunk = (struct unused_chunk *) ((char *) ptr - USED_CHUNK_METADATA_SIZE - MCHUNKPTR_SIZE);
		printf("(%-4d)  size=[%-5zu]  real_size=[%-6zu]  ptr=%p", i, size, chunk->size, ptr);
		// print_list((struct unused_chunk *) lists.heaps);
		do_free = (i % 2 == 0);
		if (do_free)
		{
			ft_free(ptr);
			printf("  freed");
		}
		printf("\n");
		i++;
	}
	return (0);
}

