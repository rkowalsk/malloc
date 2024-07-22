#include "malloc.h"

// struct lists lists;
struct unused_chunk *first_free;

struct unused_chunk	*search_free_list(size_t size)
{
	struct unused_chunk	*curr;

	curr = first_free;
	while (curr && (curr->size & SIZE_MASK) < size)
		curr = curr->fwd;
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

void	*allocate_chunk(size_t size)
{
	char				*heap;
	unsigned long		*long_ptr;
	size_t				heap_size;
	struct unused_chunk	*leftover;
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
	long_ptr = (unsigned long *) (heap + HEAP_HEADER_SIZE);
	*long_ptr = size & USED_CHUNK;
	leftover = (struct unused_chunk *) (heap + size);
	leftover->size = heap_size - size - HEAP_HEADER_SIZE;
	insert_free_list(leftover);
	return (heap);
}

void	remove_free_list(struct unused_chunk *chunk)
{
	if (chunk->fwd)
		chunk->fwd->bwd = chunk->bwd;
	if (chunk->bwd)
		chunk->bwd->fwd = chunk->fwd;
	chunk->size |= USED_CHUNK;
	* (long *) (chunk + 16) = 0x666;
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
	chunk = search_free_list(size);
	if (chunk)
		remove_free_list(chunk);
	else
		chunk = allocate_chunk(size);
//	print_list(first_free);
	print_chunk(chunk);
	return (chunk + 16);
}
