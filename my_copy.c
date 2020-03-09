#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Timer.c"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define BUFSIZE 512

double wallClockTime, userCPUTime, sysCPUTime;


void calcTime()
{
	Timer_elapsedWallclockTime(&wallClockTime);
	Timer_elapsedUserTime(&userCPUTime);
	Timer_elapsedSystemTime(&sysCPUTime);
	printf("wallclock %lf, user %lf, system time %lf\n", wallClockTime, userCPUTime, sysCPUTime);
}

int checkFile(FILE * file)
{
	if (file == NULL)
	{
		perror("Error in opening file");
		return 0;
	}
	return 1;
}

void methodOne(char * inputFile, char * outputFile)		/* EXCEEDED LINE COUNT BY 4*/
{
	Timer_start();
	FILE * file = fopen(inputFile, "r"), * o_file = fopen(outputFile, "w+");
	
	if (!checkFile(file)) 
		return;

	int c;
	while ((c = fgetc(file)) != EOF)
		fputc(c, o_file);

	fclose(o_file), fclose(file);
	calcTime();

}

void readCloseDuration(int fileD, int outputD, char * buffer, int bytes)
{
	while (read(fileD, buffer, bytes) > 0)
		write(outputD, buffer, bytes);

	close(fileD), close(outputD);
	calcTime();
}

void methodTwo(char * inputFile, char * outputFile)		/*EXCEEDED LINE COUNT BY 1*/
{
	Timer_start();
	int fileDescriptor = open(inputFile, O_RDONLY, S_IRUSR), outputDescriptor = open(outputFile, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	char letter[1];
	
	if (fileDescriptor < 0)
		return;

	readCloseDuration(fileDescriptor, outputDescriptor, letter, 1);
}

void methodThree(char * inputFile, char * outputFile)	/*EXCEEDED LINE COUNT BY 1*/
{
	Timer_start();
	int fileDescriptor = open(inputFile, O_RDONLY, S_IRUSR), outputDescriptor = open(outputFile, O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
	char buf[512];
	
	if (fileDescriptor < 0)
		return;

	readCloseDuration(fileDescriptor, outputDescriptor, buf, 512);
}

void executeWhichMethod(int methodNum, char * inputFile, char * outputFile, int numCopies)
{

	for (int i = 0; i < numCopies; ++i)
	{
		switch(methodNum)
		{
			case 1:
				methodOne(inputFile, outputFile);
				break;
			case 2:
				methodTwo(inputFile, outputFile);
				break;
			case 3:
				methodThree(inputFile, outputFile);
				break;
			default:
				break;
		}
	}
}

int main(int argc, char * argv[], char * envp[])
{
	int numCopies = (argc == 4) ? 1 : atoi(argv[4]);
	executeWhichMethod(atoi(argv[1]), argv[2], argv[3], numCopies);

	return 0;
}
