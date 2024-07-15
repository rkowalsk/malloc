#include "malloc.h"

// struct lists lists;
struct unused_chunk *first_free;

long	get_preallocation_size(long chunk_size, long page_size)
{
	long	map_size;
	long	total_chunks_size;

	chunk_size = chunk_size + USED_CHUNK_METADATA_SIZE;
	if (chunk_size < MIN)
		chunk_size = MIN;
	total_chunks_size = chunk_size * 100;
	map_size = total_chunks_size / page_size;
	map_size = (map_size + ((total_chunks_size % page_size) > 0)) * page_size;
	return (map_size);
}

void	insert_new_chunks(struct unused_chunk *new_first,
	struct unused_chunk *new_last)
{
	struct unused_chunk	*curr;
	struct unused_chunk *before;

	if (!first_free)
		first_free = new_first;
	else
	{
		curr = first_free;
		while (curr->size < new_first->size && curr->fwd)
			curr = curr->fwd;
		before = curr->bwd;
		before->fwd = new_first;
		new_first->bwd = before;
		curr->bwd = new_last;
		new_last->fwd = curr;
	}
}

// allocates a TINY or SMALL heap (give it the right flag)
int	preallocate_heap(long page_size, long heap_flag)
{
	long				map_size;
	void				*address;
	void				*end;
	struct unused_chunk	*list_curr;
	struct unused_chunk	*list_start;
	long				chunk_payload_size;
	long				full_chunk_size;
	struct unused_chunk	*chunk;

	if (IS_TINY(heap_flag))
		chunk_payload_size = TINY;
	else
		chunk_payload_size = SMALL;
	map_size = get_preallocation_size(chunk_payload_size, page_size);
	address = mmap(NULL, map_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!address)
		return (1);
	*(long *)address = map_size | heap_flag;
	dprintf(1, "%p\n", address);
	address += HEAP_HEADER_SIZE;
	full_chunk_size = chunk_payload_size + USED_CHUNK_METADATA_SIZE;
	if (full_chunk_size < MIN)
		full_chunk_size = MIN;
	end = address + map_size - full_chunk_size;
	list_start = NULL;
	list_curr = NULL;
	while (address < end)
	{
		chunk = address;
		chunk->size = full_chunk_size | heap_flag;
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
		address += full_chunk_size;
	}
	insert_new_chunks(list_start, list_curr);
	return (0);
}

void	print_list(struct unused_chunk *chunk)
{
	while (chunk != NULL)
	{
		printf("Block -> size: %ld", chunk->size & SIZE_MASK);
		printf(", usable size: %ld",
				(chunk->size & SIZE_MASK) - USED_CHUNK_METADATA_SIZE);
		if (IS_TINY(chunk->size))
			printf(", TINY");
		else if (IS_SMALL(chunk->size))
			printf(", SMALL");
		printf("\n");
		chunk = chunk->fwd;
	}
}

int	initialize_malloc(void)
{
	long	page_size;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (1);
	if (preallocate_heap(page_size, TINY_HEAP))
		return (1);
	if (preallocate_heap(page_size, SMALL_HEAP))
		return (1);
	print_list(first_free);
	return (0);
}

void	*ft_malloc(size_t size)
{
	(void) size;
	static bool	initialized = 0;
//	static long	page_size = sysconf(_SC_PAGESIZE);

	if (!initialized)
	{
		if (initialize_malloc())
			return (NULL);
		initialized = 1;
	}
	return (NULL);
}
