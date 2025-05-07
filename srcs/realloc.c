#include "ft_malloc.h"

#ifdef DEV
	void	*ft_realloc(void *ptr, size_t size)
#else
	void	*realloc(void *ptr, size_t size)
#endif
{
	char				*new;
	struct unused_chunk	*old;

#ifdef DEV
	new = ft_malloc(size);
#else
	new = malloc(size);
#endif
	if (!new || !ptr)
		return (new);
	old = (struct unused_chunk *) ((char *) ptr - MCHUNKPTR_SIZE);
	if ((old->size & SIZE_MASK) - USED_CHUNK_METADATA_SIZE < size)
		size = (old->size & SIZE_MASK) - USED_CHUNK_METADATA_SIZE;
	ft_memcpy(new, ptr, size);
#ifdef DEV
	ft_free(ptr);
#else
	free(ptr);
#endif
	return (new);
}
