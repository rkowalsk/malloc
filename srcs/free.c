#include "ft_malloc.h"

struct unused_chunk *coallesce_previous(struct unused_chunk **list, struct unused_chunk *chunk)
{
	struct unused_chunk *prev_chunk;

	prev_chunk = (struct unused_chunk *)
		((char *) chunk - (SIZE_MASK & chunk->prev_size));
	if (prev_chunk->fwd)
		prev_chunk->fwd->bwd = prev_chunk->bwd;
	if (prev_chunk->bwd)
		prev_chunk->bwd->fwd = prev_chunk->fwd;
	if (*list == prev_chunk)
		*list = prev_chunk->fwd;
	prev_chunk->size += (SIZE_MASK & chunk->size);
	return (prev_chunk);
}

// return true if the heap got unmaped
bool	coallesce_next(struct unused_chunk **list, struct unused_chunk *chunk)
{
	struct heap			*heap;
	struct unused_chunk *next_chunk;

	heap = get_chunk_heap(chunk);
	next_chunk = (struct unused_chunk *)
		((char *) chunk + (SIZE_MASK & chunk->size));
	if ((char *) next_chunk + MCHUNKPTR_SIZE >= (char *) heap + (SIZE_MASK & heap->size))
		goto unmaping;
	if (IS_USED(next_chunk->size))
		return (false);
	if (next_chunk->fwd)
		next_chunk->fwd->bwd = next_chunk->bwd;
	if (next_chunk->bwd)
		next_chunk->bwd->fwd = next_chunk->fwd;
	if (*list == next_chunk)
		*list = next_chunk->fwd;
	chunk->size += next_chunk->size & SIZE_MASK;
unmaping:
	if (lists.mmaped > UNMAP_THRESHOLD &&
		(chunk->size & SIZE_MASK) == (heap->size & SIZE_MASK) - HEAP_HEADER_SIZE)
	{
		if (heap->fwd)
			heap->fwd->bwd = heap->bwd;
		if (heap->bwd)
			heap->bwd->fwd = heap->fwd;
		if (lists.heaps == heap)
			lists.heaps = heap->fwd;
		lists.mmaped -= heap->size;
		munmap(heap, heap->size);
		return (true);
	}
	return (false);
}

struct	unused_chunk *coallesce(struct unused_chunk *chunk)
{
	struct unused_chunk	**list;

	if (IS_PREALLOC(chunk->size))
	{
		if ((chunk->size & SIZE_MASK) <= TINY)
			list = &lists.tiny;
		else
			list = &lists.small;
	}
	else
		list = &lists.large;
	if (!IS_PREV_USED(chunk->size))
		chunk = coallesce_previous(list, chunk);
	if (coallesce_next(list, chunk))
		return (NULL);
	return (chunk);
}

#ifdef DEV
	void	ft_free(void *ptr)
#else
	void	free(void *ptr)
#endif
{
	struct unused_chunk	*chunk;

	if (!ptr)
		return;
	chunk = (struct unused_chunk *)
		((char *) ptr - (MCHUNKPTR_SIZE + USED_CHUNK_METADATA_SIZE));
	chunk->size -= USED_CHUNK;
	pthread_mutex_lock(&mutex);
	chunk = coallesce(chunk);
	if (chunk)
	{
		update_next_neighbour(chunk, 0);
		if ((chunk->size & SIZE_MASK) <= TINY)
			insert_into_list(&lists.tiny, chunk);
		if ((chunk->size & SIZE_MASK) <= SMALL)
			insert_into_list(&lists.small, chunk);
		else
			insert_into_list(&lists.large, chunk);
	}
	pthread_mutex_unlock(&mutex);
}
