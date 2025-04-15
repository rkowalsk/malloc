#include "ft_malloc.h"

struct lists		lists;
bool				initialized = 0;
pthread_mutex_t		mutex = PTHREAD_MUTEX_INITIALIZER;

// the address of big chunk must be the beginning of the free space
void	*split_chunk(void *big_chunk, size_t big_size, size_t size,
		unsigned long prev_used)
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
		leftover->size = leftover_size | PREV_USED;
		insert_free_list(leftover);
		*chunk = size | USED_CHUNK | prev_used;
	}
	else
		*chunk = big_size | USED_CHUNK | prev_used;
	return ((char *) chunk - MCHUNKPTR_SIZE);
}

struct unused_chunk	*get_free_list(size_t size)
{
	struct unused_chunk	*curr;

	curr = lists.free;
	while (curr && (curr->size & SIZE_MASK) < size)
		curr = curr->fwd;
	if (curr)
	{
		remove_free_list(curr);
		if (!IS_PREALLOC(curr->size))
			curr = split_chunk((char *) curr + MCHUNKPTR_SIZE,
					curr->size & SIZE_MASK, size, curr->size & PREV_USED);
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
	insert_heap_list((struct heap *) heap);
	return (split_chunk(heap + HEAP_HEADER_SIZE, heap_size - HEAP_HEADER_SIZE,
			size, PREV_USED));
}

struct unused_chunk	*get_new_chunk(size_t size)
{
	long				page_size;
	struct unused_chunk	*chunk;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (NULL);
	if (size <= TINY)
	{
		pthread_mutex_lock(&mutex);
		if (preallocate_heap(page_size, true))
		{
			pthread_mutex_unlock(&mutex);
			return (NULL);
		}
		chunk = get_free_list(size);
		pthread_mutex_unlock(&mutex);
	}
	else if (size <= SMALL)
	{
		pthread_mutex_lock(&mutex);
		if (preallocate_heap(page_size, false))
		{
			pthread_mutex_unlock(&mutex);
			return (NULL);
		}
		chunk = get_free_list(size);
		pthread_mutex_unlock(&mutex);
	}
	else
	{
		pthread_mutex_lock(&mutex);
		chunk = allocate_chunk(size);
		pthread_mutex_unlock(&mutex);
	}
	return (chunk);
}

void	*ft_malloc(size_t size)
{
	struct unused_chunk	*chunk;

	pthread_mutex_lock(&mutex);
	if (!initialized)
	{
		if (initialize_malloc())
		{
			pthread_mutex_unlock(&mutex);
			return (NULL);
		}
		initialized = 1;
	}
	pthread_mutex_unlock(&mutex);
	size = align_size(size);
	pthread_mutex_lock(&mutex);
	chunk = get_free_list(size);
	pthread_mutex_unlock(&mutex);
	if (chunk)
	{
		pthread_mutex_lock(&mutex);
		update_next_neighbour(chunk, PREV_USED);
		pthread_mutex_unlock(&mutex);
	}
	else
		chunk = get_new_chunk(size);
	if (!chunk)
		return (NULL);
	return ((char *) chunk + USED_CHUNK_METADATA_SIZE + MCHUNKPTR_SIZE); //+16
}
