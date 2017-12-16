#include <stdlib.h>

int main()
{
	char	*ptr;

	write(1, "begin test\n", 11);
	ptr = malloc(1);
	for (int i = 0; i < 1; i++)
		ptr[i] = 42;

	ptr = realloc(ptr, 64);
	for (int i = 0; i < 64; i++)
		ptr[i] = 42;
	free(ptr);
}
