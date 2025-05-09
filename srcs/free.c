#include "ft_malloc.h"

extern unsigned long	mmaped;

struct unused_chunk *coallesce_previous(struct unused_chunk *chunk)
{
	struct unused_chunk *prev_chunk;

	prev_chunk = (struct unused_chunk *)
		((char *) chunk - (SIZE_MASK & chunk->prev_size));
	if (prev_chunk->fwd)
		prev_chunk->fwd->bwd = prev_chunk->bwd;
	if (prev_chunk->bwd)
		prev_chunk->bwd->fwd = prev_chunk->fwd;
	if (lists.free == prev_chunk)
		lists.free = prev_chunk->fwd;
	prev_chunk->size += (SIZE_MASK & chunk->size);
	return (prev_chunk);
}

// return true if the 
bool	coallesce_next(struct unused_chunk *chunk)
{
	struct heap			*heap;
	struct unused_chunk *next_chunk;

	heap = get_chunk_heap(chunk);
	next_chunk = (struct unused_chunk *)
		((char *) chunk + (SIZE_MASK & chunk->size));
	if ((char *) next_chunk + MCHUNKPTR_SIZE >= (char *) heap + heap->size)
		return (false);
	if (IS_USED(next_chunk->size))
		return (false);
	if (next_chunk->fwd)
		next_chunk->fwd->bwd = next_chunk->bwd;
	if (next_chunk->bwd)
		next_chunk->bwd->fwd = next_chunk->fwd;
	if (lists.free == next_chunk)
		lists.free = next_chunk->fwd;
	chunk->size += next_chunk->size & SIZE_MASK;
}

struct	unused_chunk *coallesce(struct unused_chunk *chunk)
{
	if (IS_PREALLOC(chunk->size))
		return (chunk);
	if (!IS_PREV_USED(chunk->size))
		chunk = coallesce_previous(chunk);
	coallesce_next(chunk);
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
	if (!IS_PREALLOC(chunk->size))
		update_next_neighbour(chunk, 0);
	insert_free_list(chunk);
	pthread_mutex_unlock(&mutex);
}
