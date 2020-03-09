#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

char * args[512];
char inputLine[512];

void printArray()
{
	for (int i = 0; args[i] != '\0'; ++i)
		printf("args[%d] is %s\n", i, args[i]);
}

void createTokens(int length)
{
	char * tok = strtok(inputLine, " ");
	while (tok != NULL)
	{
		args[length++] = tok;
		tok = strtok(NULL, " ");
	}
	args[length] = '\0';
}

void readInput()
{
	scanf(" %[^\n]", &inputLine);
	createTokens(0);
}

int isFile(char * arg)
{
	struct stat st;
	stat(arg, &st);
	return S_ISREG(st.st_mode);
}

char * findFullPath(char * p) /* Exception 4 lines */
{
	static char fullPath[512];
	const char * pathVariable = getenv("PATH");
	char path[512];
	struct stat st;
	char * token;

	sprintf(path, "%s", pathVariable);
	char * parsedToken = strtok(path, ":");

	while (parsedToken != NULL)
	{
		if (sprintf(fullPath, "%s/%s", parsedToken, p) && isFile(fullPath))
		{
			stat(fullPath, &st);
			if (st.st_mode & S_IXUSR)
				return fullPath;
		}
		parsedToken = strtok(NULL, ":");
	}
	return NULL;
}

int main(int argc, char * argv[], char * envp[]) /* EXCEPTION 2 line */
{
	readInput();
	struct stat st;

	if (isFile(args[0])) 
		execve(args[0], args, envp);
	else
	{
		args[0] = findFullPath(args[0]);
		execve(args[0], args, envp);
	}

	return 0;
}
