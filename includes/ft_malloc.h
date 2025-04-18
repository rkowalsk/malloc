#pragma once

#include "libft.h"
#include <errno.h>
#include <stdbool.h>
#include <pthread.h>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <sys/mman.h>
#include <pthread.h>

// Sizes
#define TINY 40
#define SMALL 136
#define HEAP_HEADER_SIZE 24
#define MCHUNKPTR_SIZE 8
#define USED_CHUNK_METADATA_SIZE 8
#define UNUSED_CHUNK_METADATA_SIZE 32 // ou 24 ?
#define MIN UNUSED_CHUNK_METADATA_SIZE

// Flags and masks
#define USED_CHUNK 0b001
#define PREALLOCATED 0b010
// PREV_USED isn't set for preallocated heap chunks because we
// don't coallesce them anyway and flemme
#define PREV_USED 0b100
#define SIZE_MASK 0xFFFFFFFFFFFFFFF8

#define IS_USED(value) (value & USED_CHUNK)
#define IS_PREALLOC(value) (value & PREALLOCATED)
#define IS_PREV_USED(value) (value & PREV_USED)

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

//prev_size isn't set for preallocated chunks because useless and flemme

// just the header, we don't need the rest
// the linked list doesn't loop (for now ?)
struct	unused_chunk
{
	unsigned long	prev_size; // acutally the prev_size of the previous chunk
	unsigned long	size;
	struct unused_chunk	*fwd;
	struct unused_chunk	*bwd;
};

struct heap
{
	unsigned long	size;
	struct heap		*fwd;
	struct heap		*bwd;
};

struct	lists
{
	struct unused_chunk	*free;
	struct heap			*heaps;
};

// TODO: replace all longs with fixed-width integers ?
//extern struct unused_chunk *first_free; // first free block
extern struct lists		lists;
extern pthread_mutex_t	mutex;

void		*ft_malloc(size_t size);
void		*ft_realloc(void *ptr, size_t size);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_free(void *ptr);
void		show_alloc_mem(void);
int			initialize_malloc(void);
void		print_list(struct unused_chunk *chunk);
size_t		align_size(size_t size);
void		print_list(struct unused_chunk *chunk);
void		print_chunk(struct unused_chunk *chunk);
int			preallocate_heap(long page_size, bool is_tiny);
void		insert_free_list(struct unused_chunk *chunk);
void		remove_free_list(struct unused_chunk *chunk);
void		insert_heap_list(struct heap *heap);
struct heap	*get_chunk_heap(struct unused_chunk *chunk);
void		update_next_neighbour(struct unused_chunk *chunk, bool prev_used);

//	void *a = mmap(NULL, 1, PROT_READ | PROT_WRITE,
//		MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
