#include "ft_malloc.h"

void	insert_heap_list(struct heap *heap)
{
	heap->bwd = NULL;
	heap->fwd = lists.heaps;
	if (lists.heaps)
		lists.heaps->bwd = heap;
	lists.heaps = heap;
}
