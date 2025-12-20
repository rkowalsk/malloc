#include "ft_malloc.h"

static long	get_preallocation_size(long chunk_size, long page_size)
{
	long	map_size;
	long	total_chunks_size;

	chunk_size = chunk_size + USED_CHUNK_METADATA_SIZE;
	total_chunks_size = chunk_size * 100;
	map_size = (HEAP_HEADER_SIZE + total_chunks_size) / page_size;
	map_size = (map_size + ((total_chunks_size % page_size) > 0)) * page_size;
	return (map_size);
}

static void	insert_new_chunks(struct unused_chunk *new_first,
	struct unused_chunk *new_last)
{
	struct unused_chunk	*curr;
	struct unused_chunk *next;

	if (!lists.free)
		lists.free = new_first;
	else
	{
		curr = lists.free;
		while (curr->fwd &&
				(curr->fwd->size & SIZE_MASK) < (new_last->size & SIZE_MASK))
			curr = curr->fwd;
		next = curr->fwd;
		new_first->bwd = curr;
		new_last->fwd = next;
		curr->fwd = new_first;
		if (next)
			next->bwd = new_last;
	}
}

// allocates a TINY or SMALL heap (give it 1 for tiny and 0 for small)
int	preallocate_heap(long page_size, bool is_tiny)
{
	long				map_size;
	char				*address;
	char				*curr;
	char				*end;
	char				*next;
	struct unused_chunk	*list_curr;
	struct unused_chunk	*list_start;
	long				chunk_payload_size;
	long				full_chunk_size;
	struct unused_chunk	*chunk;

	if (is_tiny)
		chunk_payload_size = TINY;
	else
		chunk_payload_size = SMALL;
	map_size = get_preallocation_size(chunk_payload_size, page_size);
	address = mmap(NULL, map_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!address)
		return (1);
	* (long *) address = map_size | PREALLOCATED;
	full_chunk_size = chunk_payload_size + USED_CHUNK_METADATA_SIZE;
	if (full_chunk_size < MIN)
		full_chunk_size = MIN;
	// potential segfault ?
	// would never happen anyway
	end = address + map_size - full_chunk_size - MCHUNKPTR_SIZE;
	list_start = NULL;
	list_curr = NULL;
	curr = address + HEAP_HEADER_SIZE - MCHUNKPTR_SIZE;
	while (curr < end)
	{
		chunk = (struct unused_chunk *) curr;
		next = curr + full_chunk_size;
		if (next >= end)
		{
			chunk->size = (address + map_size) - (curr + MCHUNKPTR_SIZE);
			chunk->size |= PREALLOCATED;
		}
		else
			chunk->size = full_chunk_size | PREALLOCATED;
		// * (long *) (chunk + chunk_payload_size) = full_chunk_size;
		if (!list_start)
		{
			list_start = chunk; 
			chunk->bwd = NULL;
		}
		else
		{
			list_curr->fwd = chunk;
			chunk->bwd = list_curr;
		}
		list_curr = chunk;
		chunk->fwd = NULL;
		curr = next;
	}
	insert_new_chunks(list_start, list_curr);
	insert_heap_list((struct heap *) address);
	return (0);
}

