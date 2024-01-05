

int main()
{
	pid_t pid = fork();

	if (pid == 0) 
	{
		char *args[] = {"/chemin/vers/le/programme", "arg1", "arg2", NULL};
		char *envp[] = {NULL};
		if (execve(args[0], args, envp) == -1) {
			perror("Erreur lors de l'exécution de execve");
			return 1;
		}
	}
	else if (pid > 0) 
	{
		printf("Le programme a été lancé avec le PID %d\n", pid);
	}
	else
	{
		perror("Erreur lors de la création du processus");
		return 1;
	}
	return 0;
}
