#include <stdlib.h>

int main()
{
	write(1, "begin\n", 6);
/*	char	*tmp = malloc(6);

	if (tmp)
	{
		*tmp = 0;
	}*/
	write(1, "end\n", 4);
	return (0);
}
