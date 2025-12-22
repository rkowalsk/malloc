#include "ft_malloc.h"

void	initialize_malloc(void)
{
	lists.tiny = NULL;
	lists.small = NULL;
	lists.large = NULL;
	lists.heaps = NULL;
	lists.mmaped = 0;
}
