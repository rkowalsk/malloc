#include "ft_malloc.h"

// add the header size and align it correctly
size_t	align_size(size_t size)
{
	size += USED_CHUNK_METADATA_SIZE;
	if (size < MIN)
		size = MIN;
	if (size % 16 != 0)
		size += 16 - (size % 16);
	return (size);
}

void	update_next_neighbour(struct unused_chunk *chunk, bool prev_used)
{
	struct unused_chunk	*neighbour;
	struct heap			*heap;
	char				*next_chunk;

	heap = get_chunk_heap(chunk);
	next_chunk = (char *) chunk + MCHUNKPTR_SIZE + (chunk->size & SIZE_MASK);
	dprintf(1, "unn: chunk = %p, chunk->size = %ld, heap = %p, heap->size = %ld, next_chunk = %p\n", chunk, chunk->size, heap, heap->size, next_chunk);
	if (next_chunk < (char *) heap + heap->size)
	{
		neighbour = (struct unused_chunk *) (next_chunk - MCHUNKPTR_SIZE);
		if (prev_used)
			neighbour->size |= PREV_USED;
		else
			neighbour->size &= ~PREV_USED;
	}
}
