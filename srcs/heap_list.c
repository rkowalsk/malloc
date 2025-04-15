#include "ft_malloc.h"

struct heap	*get_chunk_heap(struct unused_chunk *chunk)
{
	struct heap			*heap = lists.heaps;

	while (heap && ((char *) chunk < (char *) heap
			|| (char *) chunk > (char *) heap + heap->size))
		heap = heap->fwd;
	if (!heap)
	{
		write(2, "wtf???\n", 7);
		exit(2);
	}
	return (heap);
}

void	insert_heap_list(struct heap *heap)
{
	heap->bwd = NULL;
	heap->fwd = lists.heaps;
	if (lists.heaps)
		lists.heaps->bwd = heap;
	lists.heaps = heap;
}
