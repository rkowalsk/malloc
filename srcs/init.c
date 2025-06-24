#include "ft_malloc.h"

int	initialize_malloc(void)
{
	long	page_size;

	lists.free = NULL;
	lists.heaps = NULL;
	lists.mmaped = 0;
	page_size = sysconf(_SC_PAGESIZE);
	if (page_size == -1)
		return (1);
	if (preallocate_heap(page_size, true))
		return (1);
	if (preallocate_heap(page_size, false))
		return (1);
	return (0);
}
