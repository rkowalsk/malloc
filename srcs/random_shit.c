#include "ft_malloc.h"

int posix_memalign(void **memptr, size_t alignment, size_t size)
{
	void	*ptr;

	(void) alignment;
#ifdef DEV
	ptr = ft_malloc(size);
#else
	ptr = malloc(size);
#endif
	if (!ptr)
		return (errno);
	*memptr = ptr;
	return (0);
}

void *aligned_alloc(size_t alignment, size_t size)
{
	void	*ptr;

	(void) alignment;
#ifdef DEV
	ptr = ft_malloc(size);
#else
	ptr = malloc(size);
#endif
	return (ptr);
}

void *memalign(size_t alignment, size_t size)
{
	void	*ptr;

	(void) alignment;
#ifdef DEV
	ptr = ft_malloc(size);
#else
	ptr = malloc(size);
#endif
	return (ptr);
}

void *valloc(size_t size)
{
	void	*ptr;

#ifdef DEV
	ptr = ft_malloc(size);
#else
	ptr = malloc(size);
#endif
	return (ptr);
}

void *pvalloc(size_t size)
{
	void	*ptr;

#ifdef DEV
	ptr = ft_malloc(size);
#else
	ptr = malloc(size);
#endif
	return (ptr);
}

