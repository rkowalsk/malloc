#include "ft_malloc.h"

void	ft_free(void *ptr)
{
	struct unused_chunk	*chunk;

	if (!ptr)
		return;
	chunk = (struct unused_chunk *) ((char *) ptr - (MCHUNKPTR_SIZE + USED_CHUNK_METADATA_SIZE));
	chunk->size -= USED_CHUNK;
	pthread_mutex_lock(&mutex);
	insert_free_list(chunk);
	pthread_mutex_unlock(&mutex);
}
