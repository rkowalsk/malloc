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

void	print_chunk(struct unused_chunk *chunk)
{
	dprintf(1, "Block -> address: %p", chunk);
	dprintf(1, ", size: %ld", chunk->size & SIZE_MASK);
	dprintf(1, ", usable size: %ld",
			(chunk->size & SIZE_MASK) - USED_CHUNK_METADATA_SIZE);
	if (IS_USED(chunk->size))
		dprintf(1, ", USED");
	else
		dprintf(1, ", UNUSED");
	dprintf(1, ", bwd : %p", chunk->bwd);
	dprintf(1, ", fwd : %p", chunk->fwd);
	if (IS_PREALLOC(chunk->size))
		dprintf(1, ", TINY/SMALL");
	dprintf(1, "\n");
}

void	print_list(struct unused_chunk *chunk)
{
	while (chunk != NULL)
	{
		print_chunk(chunk);
		chunk = chunk->fwd;
	}
}

