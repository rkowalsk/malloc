#include "ft_malloc.h"

#ifdef DEV
	void	*ft_calloc(size_t n, size_t size)
#else
	void	*calloc(size_t n, size_t size)
#endif
{
	char	*ptr;
	size_t	total_size;

	if (n > SIZE_MAX / size)
	{
		errno = ENOMEM;
		return (NULL);
	}
	total_size = n * size;
	#ifdef DEV
	ptr = ft_malloc(total_size);
	#else
	ptr = malloc(total_size);
	#endif
	if (ptr)
		ft_bzero(ptr, total_size);
	return (ptr);
}
