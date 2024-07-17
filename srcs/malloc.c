#include "malloc.h"

// struct lists lists;
struct unused_chunk *first_free;

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

struct unused_chunk	*search_free_list(size_t size)
{
	struct unused_chunk	*curr;

	curr = first_free;
	while (curr && (curr->size & SIZE_MASK) < size)
		curr = curr->fwd;
	return (curr);
}

size_t	new_heap_size(size_t needed_size)
{
	long	page_size;
	size_t	heap_size;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (-1);
	needed_size += HEAP_HEADER_SIZE;
	heap_size = needed_size / page_size;
	heap_size = (heap_size + ((needed_size % page_size) > 0)) * page_size;
	return (heap_size);
}
		
struct unused_chunk	*allocate_chunk(size_t size)
{
	unsigned long	*heap;
	size_t			heap_size;

	heap_size = new_heap_size(size);
	if (heap_size == -1)
		return (NULL);
	heap = mmap(NULL,  heap_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!heap)
		return (NULL);
	*heap = heap_size;
	


size_t	align_size(size_t size)
{
	size += USED_CHUNK_METADATA_SIZE;
	if (size < MIN - USED_CHUNK_METADATA_SIZE)
		size = MIN - USED_CHUNK_METADATA_SIZE;
	if (size % 16)
		size += 16 - (size % 16);
	return (size);
}

void	*ft_malloc(size_t size)
{
	(void) 				size;
	static bool			initialized = 0;
	struct unused_chunk	chunk;

	if (!initialized)
	{
		if (initialize_malloc())
			return (NULL);
		initialized = 1;
	}
//	printf("%zd ", size);
	size = align_size(size);
//	printf("%zd\n", size);
	chunk = search_free_list(size);
	if (!chunk)
		chunk = allocate_chunk(size);
	// get_free_block
	// init_free_block
	return (NULL);
}
