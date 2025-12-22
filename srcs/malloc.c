#include "ft_malloc.h"

struct lists		lists;
pthread_mutex_t		mutex = PTHREAD_MUTEX_INITIALIZER;

// the address of big chunk must be the beginning of the free space
void	*split_chunk(struct unused_chunk **list, void *big_chunk,
				size_t big_size, size_t size, unsigned long prev_used, unsigned long preallocated)
{
	unsigned long		*chunk;
	size_t				leftover_size;
	struct unused_chunk	*leftover;

	chunk = (unsigned long *) big_chunk;
	leftover_size = big_size - size;
	if (leftover_size >= MIN)
	{
		leftover = (struct unused_chunk *)
			((char *) big_chunk + size - MCHUNKPTR_SIZE);
		leftover->size = leftover_size | PREV_USED | preallocated;
		insert_into_list(list, leftover);
		*chunk = size | USED_CHUNK | prev_used | preallocated;
	}
	else
		*chunk = big_size | USED_CHUNK | prev_used | preallocated;
	return ((char *) chunk - MCHUNKPTR_SIZE);
}

struct unused_chunk	*get_from_list(struct unused_chunk **list, size_t size)
{
	struct unused_chunk	*curr;
	unsigned long	prealloc;

	prealloc = (list != &lists.large) ? PREALLOCATED : 0;
	curr = *list;
	while (curr && (curr->size & SIZE_MASK) < size)
		curr = curr->fwd;
	if (curr)
	{
		remove_from_list(list, curr);
		curr = split_chunk(list, (char *) curr + MCHUNKPTR_SIZE,
				curr->size & SIZE_MASK, size, curr->size & PREV_USED, prealloc);
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

// void	*allocate_chunk(size_t map_size, size_t chunk_size)
// {
// 	char				*heap;
// 	unsigned long		*long_ptr;
// 	size_t				heap_size;
// 	long				page_size;

// 	page_size = sysconf(_SC_PAGESIZE);
// 	if (page_size == -1)
// 		return (NULL);
// 	heap_size = new_heap_size(size, page_size);
// 	heap = mmap(NULL, heap_size, PROT_READ | PROT_WRITE,
// 			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
// 	if (!heap)
// 		return (NULL);
// 	lists.mmaped += heap_size;
// 	long_ptr = (unsigned long *) heap;
// 	*long_ptr = heap_size;
// 	insert_heap_list((struct heap *) heap);
// 	return (split_chunk(lists.heap + HEAP_HEADER_SIZE, heap_size - HEAP_HEADER_SIZE,
// 			size, PREV_USED));
// }

// struct unused_chunk	*get_new_chunk(size_t size)
// {
// 	long				page_size;
// 	struct unused_chunk	*chunk;

// 	page_size = sysconf(_SC_PAGESIZE);
// 	if (page_size == -1)
// 		return (NULL);
// 	if (size <= TINY)
// 	{
// 		if (preallocate_heap(page_size, true))
// 		{
// 			return (NULL);
// 		}
// 		chunk = get_from_list(&lists.tiny);
// 	}
// 	else if (size <= SMALL)
// 	{
// 		if (preallocate_heap(page_size, false))
// 		{
// 			return (NULL);
// 		}
// 		chunk = get_from_list(&lists.small);
// 	}
// 	else
// 	{
// 		chunk = allocate_large_chunk(&lists.large, size);
// 	}
// 	return (chunk);
// }

static long	get_preallocation_size(long chunk_size, long page_size)
{
	long	map_size;
	long	total_chunks_size;

	total_chunks_size = chunk_size * 100;
	map_size = (HEAP_HEADER_SIZE + total_chunks_size) / page_size;
	map_size = (map_size + ((total_chunks_size % page_size) > 0)) * page_size;
	return (map_size);
}

void	*allocate_new_chunk(size_t size)
{
	struct unused_chunk	*chunk;
	struct heap			*heap;
	long				map_size;
	long				page_size;
	unsigned long		prealloc;
	struct unused_chunk	**list;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (NULL);
	prealloc = 0;
	if (size <= TINY)
	{
		list = &lists.tiny;
		map_size = get_preallocation_size(TINY, page_size);
		prealloc = PREALLOCATED;
	}
	else if (size <= SMALL)
	{
		list = &lists.small;
		map_size = get_preallocation_size(TINY, page_size);
		prealloc = PREALLOCATED;
	}
	else
	{
		list = &lists.large;
		map_size = new_heap_size(size, page_size);
	}
	heap = mmap(NULL, map_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!heap)
		return (NULL);
	lists.mmaped += map_size;
	heap->size = map_size;
	if (size <= SMALL)
		heap->size |= PREALLOCATED;
	insert_heap_list(heap);
	chunk = split_chunk(list, (char *) heap + HEAP_HEADER_SIZE,
		map_size - HEAP_HEADER_SIZE, size, PREV_USED, prealloc);
	return (chunk);
}

void	*get_free_chunk(size_t size)
{
	struct unused_chunk	*chunk;

	if (size <= TINY)
		chunk = get_from_list(&lists.tiny, size);
	else if (size <= SMALL)
		chunk = get_from_list(&lists.small, size);
	else
		chunk = get_from_list(&lists.large, size);
	return (chunk);
}

#ifdef DEV
	void	*ft_malloc(size_t size)
#else
	void	*malloc(size_t size)
#endif
{
	struct unused_chunk	*chunk;
	static bool			initialized_malloc = 0;

	if (size > PTRDIFF_MAX)
	{
		errno = ENOMEM;
		return (NULL);
	}
	pthread_mutex_lock(&mutex);
	if (!initialized_malloc)
	{
		initialize_malloc();
		initialized_malloc = 1;
	}
	size = align_size(size);
	if (size <= TINY)
	chunk = get_free_chunk(size);
	if (!chunk)
		chunk = allocate_new_chunk(size);
	if (chunk)
		update_next_neighbour(chunk, PREV_USED);
	pthread_mutex_unlock(&mutex);
	if (!chunk)
		return (NULL);
	return ((char *) chunk + USED_CHUNK_METADATA_SIZE + MCHUNKPTR_SIZE); //+16
}
