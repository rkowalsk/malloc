#include "malloc.h"

// struct lists lists;
void *first_free;

long	get_preallocation_size(long chunk_size, long page_size)
{
	long	map_size;
	long	total_chunks_size;

	chunk_size = chunk_size + USED_CHUNK_METADATA_SIZE;
	if (chunk_size < UNUSED_CHUNK_METADATA_SIZE)
		chunk_size = UNUSED_CHUNK_METADATA_SIZE;
	total_chunks_size = chunk_size * 100;
	map_size = total_chunks_size / page_size;
	map_size = (map_size + ((total_chunks_size % page_size) > 0)) * page_size;
	return (map_size);
}

void	initialize_heap(void *address, long size)
{
	long *tmp;

	tmp = address;
	*tmp = size

int	initialize_malloc(void)
{
	long	page_size;
	long	map_size;
	void	*heap;

	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (1);
	map_size = get_preallocation_size(TINY, page_size);
//	printf("%ld\n", map_size);
	heap = mmap(NULL, map_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (!lists.heaps)
		return (1);
	
	lists.heaps = mmap(NULL, map_size, PROT_READ | PROT_WRITE,
			MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
