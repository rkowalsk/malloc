#include "ft_malloc.h"
#include <stdbool.h>
#include <stddef.h>

void	print_chunk(struct unused_chunk *chunk)
{
	ft_printf("Block -> address: %p", chunk);
	ft_printf(", size: %ld", chunk->size & SIZE_MASK);
	ft_printf(", usable size: %ld",
			(chunk->size & SIZE_MASK) - USED_CHUNK_METADATA_SIZE);
	if (IS_USED(chunk->size))
		ft_printf(", USED");
	else
		ft_printf(", UNUSED");
	ft_printf(", bwd : %p", chunk->bwd);
	ft_printf(", fwd : %p", chunk->fwd);
	if (IS_PREALLOC(chunk->size))
		ft_printf(", TINY/SMALL");
	ft_printf("\n");
}

void	print_list(struct unused_chunk *chunk)
{
	while (chunk != NULL)
	{
		print_chunk(chunk);
		chunk = chunk->fwd;
	}
}

void	print_heap_type(unsigned long size, struct heap *heap)
{
	if (IS_PREALLOC(size))
	{
		if ((SIZE_MASK & size) - USED_CHUNK_METADATA_SIZE == TINY)
			ft_printf("TINY: %p\n", heap);
		else
			ft_printf("SMALL: %p\n", heap);
	}
	else
		ft_printf("LARGE: %p\n", heap);
}

void	hex_dump(char *start, unsigned long size)
{
	size_t	i = 0;
	size_t	column_size = 0;

	while (i < size)
	{
		if (column_size == 0)
			ft_printf("  ");
		else
			ft_printf(" ");
		ft_printf("0x%02x", start[i]);
		i++;
		column_size++;
		if (column_size == HEX_DUMP_COLUMNS)
		{
			ft_printf("\n");
			column_size = 0;
		}
	}
	ft_printf("\n");
}

void	heap_show_alloc_mem(struct heap *heap, bool ex)
{
	struct unused_chunk	*chunk;
	bool				printed_type;

	chunk = (struct unused_chunk *) (((char *) heap) + HEAP_HEADER_SIZE - MCHUNKPTR_SIZE);
	printed_type = false;
	while ((char *) chunk < (char *) heap + (SIZE_MASK & heap->size))
	{
		if (IS_USED(chunk->size))
		{
			if (!printed_type)
			{
				print_heap_type(chunk->size, heap);
				printed_type = 1;
			}
			ft_printf(" -> %p to %p (%lu bytes)\n",
				(char *) chunk + USED_CHUNK_METADATA_SIZE + MCHUNKPTR_SIZE,
				(char *) chunk + USED_CHUNK_METADATA_SIZE + MCHUNKPTR_SIZE + (SIZE_MASK & chunk->size),
				(SIZE_MASK & chunk->size) - USED_CHUNK_METADATA_SIZE);
			if (ex)
			{
				ft_printf(" hex dump:\n");
				hex_dump(((char *) chunk) + MCHUNKPTR_SIZE + USED_CHUNK_METADATA_SIZE, (SIZE_MASK & chunk->size) - USED_CHUNK_METADATA_SIZE);
			}
		}
		chunk = (struct unused_chunk *)
			((char *) chunk + (SIZE_MASK & chunk->size) + USED_CHUNK_METADATA_SIZE);
	}
}

void	show_alloc_mem_ex(void)
{
	struct heap	*curr;

	curr = lists.heaps;
	while (curr)
	{
		heap_show_alloc_mem(curr, true);
		curr = curr->fwd;
	}
}

void	show_alloc_mem(void)
{
	struct heap	*curr;

	curr = lists.heaps;
	while (curr)
	{
		heap_show_alloc_mem(curr, false);
		curr = curr->fwd;
	}
}
