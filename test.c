#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void	*ft_malloc(size_t size);
void	ft_free(void *ptr);

static void	test_simple(void)
{
	char	*ptr1;
	char	*ptr2;

	printf("=== Test simple ===\n");

	ptr1 = (char *)ft_malloc(32);
	strcpy(ptr1, "Hello ft_malloc");
	printf("ft_malloc : %s\n", ptr1);
	ft_free(ptr1);

	ptr2 = (char *)malloc(32);
	strcpy(ptr2, "Hello malloc");
	printf("malloc    : %s\n", ptr2);
	free(ptr2);
}

static void	test_multiple_alloc(void)
{
	void	*ptrs_ft[100];
	void	*ptrs_std[100];
	int		i;

	printf("\n=== Test multiple alloc ===\n");

	i = 0;
	while (i < 100)
	{
		ptrs_ft[i] = ft_malloc(i + 1);
		ptrs_std[i] = malloc(i + 1);
		if (!ptrs_ft[i] || !ptrs_std[i])
			printf("Allocation failed at %d\n", i);
		memset(ptrs_ft[i], 'A' + (i % 26), i);
		memset(ptrs_std[i], 'A' + (i % 26), i);
		i++;
	}

	i = 0;
	while (i < 100)
	{
		ft_free(ptrs_ft[i]);
		free(ptrs_std[i]);
		i++;
	}
}

static void	test_random_alloc_free(void)
{
	void	*ptrs_ft[50];
	void	*ptrs_std[50];
	int		i;
	int		index;

	printf("\n=== Test random alloc/free ===\n");
	srand((unsigned int)time(NULL));

	memset(ptrs_ft, 0, sizeof(ptrs_ft));
	memset(ptrs_std, 0, sizeof(ptrs_std));

	i = 0;
	while (i < 1000)
	{
		index = rand() % 50;
		if (ptrs_ft[index])
		{
			ft_free(ptrs_ft[index]);
			free(ptrs_std[index]);
			ptrs_ft[index] = NULL;
			ptrs_std[index] = NULL;
		}
		else
		{
			ptrs_ft[index] = ft_malloc((rand() % 128) + 1);
			ptrs_std[index] = malloc((rand() % 128) + 1);
		}
		i++;
	}

	i = 0;
	while (i < 50)
	{
		if (ptrs_ft[i])
			ft_free(ptrs_ft[i]);
		if (ptrs_std[i])
			free(ptrs_std[i]);
		i++;
	}
}

static void	test_edge_cases(void)
{
	void	*ptr;

	printf("\n=== Test edge cases ===\n");

	ptr = ft_malloc(0);
	printf("ft_malloc(0) returned %p\n", ptr);
	ft_free(ptr);

	ptr = ft_malloc(1);
	ft_free(ptr);
	ft_free(ptr);
	printf("Double free done (should not crash)\n");
}

int	main(void)
{
	test_simple();
	test_multiple_alloc();
	test_random_alloc_free();
	test_edge_cases();
	return (0);
}

