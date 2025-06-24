#include "ft_malloc.h"

#ifdef DEV
size_t ft_malloc_usable_size(void *ptr)
#else
size_t malloc_usable_size(void *ptr)
#endif
{
	struct unused_chunk	*chunk;

	if (!ptr)
		return (0);
	chunk = (struct unused_chunk *)
		((char *) ptr - USED_CHUNK_METADATA_SIZE - MCHUNKPTR_SIZE);
	return (chunk->size);
}
