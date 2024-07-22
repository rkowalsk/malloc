#pragma once

#include "libft.h"
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>

// Sizes
#define TINY 40
#define SMALL 136
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

// Checking the values of SMALL and TINY in case someone fails to do their job
#if (TINY % 16 == 0 || TINY % 8 != 0)
	#error "TINY is not properly aligned"
#endif
#if (SMALL % 16 == 0 || SMALL % 8 != 0)
	#error "SMALL is not properly aligned"
#endif
#if (TINY < MIN - USED_CHUNK_METADATA_SIZE)
	#error "TINY is too small"
#endif
#if (SMALL < MIN - USED_CHUNK_METADATA_SIZE)
	#error "SMALL is too small"
#endif
#if (TINY >= SMALL)
	#error "TINY is ≥ SMALL bruh..."
#endif

// Unused chunk

/*   mchunkptr  */ // doesn't really exist, just the beginning of the chunk
/* size | flags */
/*     fwd      */
/*     bwd      */
/*     size     */

// just the header, we don't need the rest
// the linked list doesn't loop (for now ?)
struct	unused_chunk
{
	unsigned long	prev_size;
	unsigned long	size;
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
// extern pthread_mutex_t	mutex;

void	*ft_malloc(size_t size);
void	*ft_realloc(void *ptr, size_t size);
void	*ft_calloc(size_t nmemb, size_t size);
void	ft_free(void *ptr);
void	show_alloc_mem(void);
int		initialize_malloc(void);
void	print_list(struct unused_chunk *chunk);
size_t	align_size(size_t size);
void	print_list(struct unused_chunk *chunk);
void	print_chunk(struct unused_chunk *chunk);

//	void *a = mmap(NULL, 1, PROT_READ | PROT_WRITE,
//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
