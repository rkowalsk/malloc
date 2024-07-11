#pragma once

#include "libft.h"
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>

// Sizes
#define TINY 40
#define SMALL 128
#define MIN 24 // ou 32 ?
#define HEAP_HEADER_SIZE 16
#define USED_CHUNK_METADATA_SIZE 8
#define UNUSED_CHUNK_METADATA_SIZE 32

#define USED_CHUNK 0b001
#define TINY_HEAP 0b010
#define SMALL_HEAP 0b100

#define IS_USED(value) (value & USED_CHUNK)
#define IS_TINY(value) (value & TINY_HEAP)
#define IS_SMALL(value) (value & SMALL_HEAP)

struct	lists
{
	void	*free;
	void	*heaps;
};

// extern struct lists lists;
extern void *first_free; // first free block
pthread_mutex_t	mutex;

void	*ft_malloc(size_t size);
void	*ft_realloc(void *ptr, size_t size);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free(void *ptr);
void	show_alloc_mem(void);

//	void *a = mmap(NULL, 1, PROT_READ | PROT_WRITE,
//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
