#include "ft_malloc.h"
#include <pthread.h>
#include <stdlib.h>

void cspam(char *start, char c, int size)
{
	int i = 0;
	while (i < size)
	{
		start[i] = c;
		i++;
	}
}

void	*function(void *ptr)
{
	char *alloc;
	unsigned long	size;

	while (true)
	{
		size = rand() % 100000;
		alloc = ft_malloc(size);
		if (!alloc)
		{
			perror("malloc");
			pthread_exit(NULL);
		}
		cspam(alloc, * (char *) ptr, size);
		write(1, alloc, size);
		ft_free(alloc);
	}
	pthread_exit(NULL);
}

int main(void)
{
	char		c[25];
	pthread_t	threads[25];
	for (int i = 0; i < 25; i++)
	{
		c[i] = 'a' + i;
		if (pthread_create(threads + i, NULL, function, c + i) != 0)
		{
			perror("pthread_create");
			return (2);
		}
	}
	for (int i = 0; i < 25; i++)
		pthread_join(threads[i], NULL);
	return (0);
}
