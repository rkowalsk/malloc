#include "ft_malloc.h"

void	insert_into_list(struct unused_chunk **list, struct unused_chunk *chunk)
{
	struct unused_chunk	*curr;
	unsigned long		*prev_size;

	if (!*list)
	{
		chunk->bwd = NULL;
		chunk->fwd = NULL;
		*list = chunk;
	}
	else if (((*list)->size & SIZE_MASK) >= (chunk->size & SIZE_MASK))
	{
		(*list)->bwd = chunk;
		chunk->bwd = NULL;
		chunk->fwd = *list;
		*list = chunk;
	}
	else
	{
		curr = *list;
		while (curr->fwd &&
				(curr->fwd->size & SIZE_MASK) < (chunk->size & SIZE_MASK))
			curr = curr->fwd;
		chunk->fwd = curr->fwd;
		chunk->bwd = curr;
		if (curr->fwd)
			curr->fwd->bwd = chunk;
		curr->fwd = chunk;
	}
	prev_size = (unsigned long *) ((char *) chunk + (chunk->size & SIZE_MASK));
	*prev_size = chunk->size;
}

void	remove_from_list(struct unused_chunk **list, struct unused_chunk *chunk)
{
	if (chunk->fwd)
		chunk->fwd->bwd = chunk->bwd;
	if (chunk->bwd)
		chunk->bwd->fwd = chunk->fwd;
	chunk->size |= USED_CHUNK;
	if (chunk == *list)
		*list = chunk->fwd;
}
