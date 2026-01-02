#include "ft_malloc.h"

struct heap	*get_chunk_heap(struct unused_chunk *chunk)
{
	struct heap	*heap = lists.heaps;

	while (heap && ((char *) chunk < (char *) heap
			|| (char *) chunk > (char *) heap + (SIZE_MASK & heap->size)))
		heap = heap->fwd;
	if (!heap)
	{
		write(2, "wtf???\n", 7);
	}
	return (heap);
}

// the list is sorted by address
void	insert_heap_list(struct heap *heap)
{
	struct heap	*curr;

	if (!lists.heaps)
	{
		heap->bwd = NULL;
		heap->fwd = NULL;
		lists.heaps = heap;
	}
	else if (lists.heaps > heap)
	{
		heap->bwd = NULL;
		heap->fwd = lists.heaps;
		lists.heaps->bwd = heap;
		lists.heaps = heap;
	}
	else
	{
		curr = lists.heaps;
		while (curr->fwd && curr->fwd < heap)
			curr = curr->fwd;
		heap->bwd = curr;
		heap->fwd = curr->fwd;
		if (curr->fwd)
			curr->fwd->bwd = heap;
		curr->fwd = heap;
	}
}
