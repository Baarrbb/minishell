#include <stdio.h>
int	main(void)
{
	char	*test = "?=\0";
	printf("%s\n%d\n",test, strlen(test));
}