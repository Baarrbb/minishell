#include <stdio.h>

int main (int ac, char **av, char **envp)
{
	if (!envp)
		printf("NULL\n");
	if (!envp[0])
		printf("1111NULL\n");
	for (int i = 0; envp[i]; i++)
		printf("%d : %s\n", i, envp[i]);
}