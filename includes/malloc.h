#pragma once

#include "libft.h"
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>

// Sizes
#define TINY 32
#define SMALL 128
#define HEAP_HEADER_SIZE 8 // ou 16 ?
#define USED_CHUNK_METADATA_SIZE 8
#define UNUSED_CHUNK_METADATA_SIZE 32 // ou 24 ?
#define MIN UNUSED_CHUNK_METADATA_SIZE

// Flags
#define USED_CHUNK 0b001
#define TINY_HEAP 0b010
#define SMALL_HEAP 0b100
#define SIZE_MASK 0xFFFFFFFFFFFFFFF8

#define IS_USED(value) (value & USED_CHUNK)
#define IS_TINY(value) (value & TINY_HEAP)
#define IS_SMALL(value) (value & SMALL_HEAP)

// When going through the free list, use this on an element to get its size
#define L_CHUNK_SIZE(address_in_list) (* (long *) (address_in_list - 8) )

// Unused chunk

/* size | flags */
/*     fwd      */
/*     bwd      */
/*     size     */

// just the header, we don't need the rest
// the linked list doesn't loop (for now ?)
struct	unused_chunk
{
	long	size;
	struct unused_chunk	*fwd;
	struct unused_chunk	*bwd;
};

struct	lists
{
	void	*free;
	void	*heaps;
};

// extern struct lists lists;
extern struct unused_chunk *first_free; // first free block
pthread_mutex_t	mutex;

void	*ft_malloc(size_t size);
void	*ft_realloc(void *ptr, size_t size);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free(void *ptr);
void	show_alloc_mem(void);

//	void *a = mmap(NULL, 1, PROT_READ | PROT_WRITE,
//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
