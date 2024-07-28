#include "malloc.h"

// struct lists lists;
struct unused_chunk *first_free;

void	insert_free_list(struct unused_chunk *chunk)
{
	struct unused_chunk	*curr;

	if (!first_free)
		first_free = chunk;
	else
	{
		curr = first_free;
		while (curr->fwd && curr->fwd->size < (chunk->size & SIZE_MASK))
			curr = curr->fwd;
		chunk->fwd = curr->fwd;
		chunk->bwd = curr;
		if (curr->fwd)
			curr->fwd->bwd = chunk;
		curr->fwd = chunk;
	}
}

void	remove_free_list(struct unused_chunk *chunk)
{
	if (chunk->fwd)
		chunk->fwd->bwd = chunk->bwd;
	if (chunk->bwd)
		chunk->bwd->fwd = chunk->fwd;
	chunk->size |= USED_CHUNK;
	if (chunk == first_free)
		first_free = chunk->fwd;
	//	* (long *) (chunk + 16) = 0x666;
}

void	*split_chunk(void *big_chunk, size_t big_size, size_t size)
{
	unsigned long		*chunk;
	size_t				leftover_size;
	struct unused_chunk	*leftover;

	chunk = (unsigned long *) ((char *) big_chunk + HEAP_HEADER_SIZE);
	leftover_size = big_size - size - HEAP_HEADER_SIZE;
	if (leftover_size >= MIN)
	{
		leftover = (struct unused_chunk *) ((char *)big_chunk + size);
		leftover->size = leftover_size;
		insert_free_list(leftover);
		*chunk = size | USED_CHUNK;
	}
	else
		*chunk = (big_size - HEAP_HEADER_SIZE) | USED_CHUNK;
	return ((char *) chunk - HEAP_HEADER_SIZE);
}

struct unused_chunk	*get_free_list(size_t size)
{
	struct unused_chunk	*curr;

	curr = first_free;
	while (curr && (curr->size & SIZE_MASK) < size)
		curr = curr->fwd;
	if (curr)
	{
		remove_free_list(curr);
		if (!IS_SMALL(curr->size) && !IS_TINY(curr->size))
			curr = split_chunk(curr, curr->size & SIZE_MASK, size);
	}
	return (curr);
}

size_t	new_heap_size(size_t needed_size, long page_size)
{
	size_t	heap_size;

	needed_size += HEAP_HEADER_SIZE;
	heap_size = needed_size / page_size;
	heap_size = (heap_size + ((needed_size % page_size) > 0)) * page_size;
	return (heap_size);
}

void	*allocate_chunk(size_t size)
{
	char				*heap;
	unsigned long		*long_ptr;
	size_t				heap_size;
	long				page_size;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (NULL);
	heap_size = new_heap_size(size, page_size);
	heap = mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!heap)
		return (NULL);
	long_ptr = (unsigned long *) heap;
	*long_ptr = heap_size;
	return (split_chunk(heap, heap_size - HEAP_HEADER_SIZE, size));
}

void	*ft_malloc(size_t size)
{
	static bool			initialized = 0;
	struct unused_chunk	*chunk;

	if (!initialized)
	{
		if (initialize_malloc())
			return (NULL);
		initialized = 1;
	}
	//	dprintf(1, "%zd ", size);
	size = align_size(size);
	dprintf(1, "%zd\n", size);
	chunk = get_free_list(size);
	if (!chunk)
		chunk = allocate_chunk(size);
	//	print_list(first_free);
	//	print_chunk(chunk);
	return ((char *) chunk + USED_CHUNK_METADATA_SIZE + HEAP_HEADER_SIZE); // + 16
}
