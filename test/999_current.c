#include <stdlib.h>

int main()
{
	char	*ptr;
	int		sizes[] = {
		696,
		50,
		977,
		36511447,
		30,
		16
	};

	for (int i = 0; i < sizeof(sizes) / *sizes; i++)
	{
		ptr = malloc(sizes[i]);
		arc4random_buf(ptr, sizes[i]);
	}
}
