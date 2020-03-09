#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


char letters[5] = {'A', 'B', 'C', 'D'};
int processes = 4;


int start_child_printing_k_char(char letter, int number)
{
	for (int i = 0; i < number; ++i)
	{	
		putchar(letter);
		fflush(stdout);
	}
	return 0;
}

void wait_for_all_children()
{
	int status;
	while(wait(&status) >= 0)
	{
	}
	printf("\n");
}

int main(int argc, char * argv[]) /*EXCEEDED 3 lines */
{
	int k = (argc == 1) ? 1000 : atoi(argv[1]);
	for (int i = 0; i < processes; ++i)
	{
		int pid = fork();
		switch (pid)
		{
			case -1:
				printf("Unsuccessful fork\n");
				exit(-1);
			case 0:
				return start_child_printing_k_char(letters[i], k);

			default:
				break;
		}
	}
	wait_for_all_children();
	return 0;
}
