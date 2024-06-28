#include "ft_malloc.h"

void *malloc(size_t size)
{
	(void) size;
	ft_putstr_fd("prankex", 1);
	errno = ENOMEM;
	return (NULL);
}

void	free(void *ptr)
{
	(void) ptr;
	ft_putstr_fd("prankex", 1);
}


void	*realloc(void *ptr, size_t size)
{
	(void) ptr;
	(void) size;
	errno = ENOMEM;
	return (NULL);
}

void	*calloc(size_t nmemb, size_t size)
{
	(void) nmemb;
	(void) size;
	errno = ENOMEM;
	return (NULL);
}

