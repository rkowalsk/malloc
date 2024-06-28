#pragma once

#include "libft.h"
#include <errno.h>

void	*malloc(size_t size);
void	*realloc(void *ptr, size_t size);
void	*calloc(size_t nmemb, size_t size);
void	free(void *ptr);
void	show_alloc_mem(void);
