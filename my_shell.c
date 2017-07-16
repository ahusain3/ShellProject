//Name Ahsen Husain
//ID 0895117
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include <math.h>
#include <stddef.h>

#define BUF_SIZE 512

extern char **getln();
void execute(char** , int);
void loop(void);
void add(char**, int);
void foregroundCommand(char**);
void backgroundCommand(char**);
int isOutputRedirect (char**, int);
void redirect(char**, int, int);
void readMyself( int argc, char *argv[]);
void reap(int);
int isInputRedirect ( char**, int);
void showArgs(char** , int );
int hexConverter(char* );
void argsBackwards(char** , int);

int main(void)
{
	loop();
	return 0;
}

void loop(void) {
	int i;
	int size = 0;
	char **args; 
	while(1) {
		//print prompt
		printf(">> ");
		//read and parse line into array of arguments
		args = getln();
		for(i = 0; args[i] != NULL; i++) {
			size++;	
		}
		//execute
		execute(args, size);
		//reset size to 0 for next line
		size = 0;
		
		      
	}
}
 
/*
void readMyself( int argc, char *argv[])
{
	int bufferSize = BUF_SIZE;
	int index = 0;
	char* buffer = malloc(sizeof(char) * BUF_SIZE);
	int c;
}
*/


void execute(char** args, int size)
{
	

	int indexOfOut = isOutputRedirect(args, size);
	int indexOfIn = isInputRedirect(args, size);
	
	if (size < 1)
	{
		return;
	}
	else if( strcmp(args[0], "add") == 0 )
	{
	    add(args, size);
	}
	else if( strcmp(args[0], "exit") == 0 )
	{
		exit(1);
	}
	else if( strcmp(args[size - 1], "&") == 0 )
	{
		//reap the zombie when child has terminated
		if ( signal(SIGCHLD, reap) == SIG_ERR )
		{
			printf("Signal function failed\n");
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}
		
		args[size - 1] = NULL;
		backgroundCommand(args);
	}
	else if ( indexOfOut != 0 || indexOfIn != 0)
	{
		redirect(args, indexOfOut, indexOfIn);
	}
	else if ( strcmp(args[0], "show") == 0)
	{
		showArgs(args, size);
	}
	else if ( strcmp(args[0], "backwards") == 0)
	{
		//call argsBackwards
		argsBackwards(args, size);
	}
	else
	{
		foregroundCommand(args);
	}
}

void argsBackwards(char** args, int size)
{
	if (size == 1)
	{
		printf("Please enter more arguments.\n");
		return;
	}
	int i;
	for ( i = size - 1; i >= 1; i--)
	{
		printf("%s\n", args[i]);
	}
	return;
}
	
void showArgs(char** args, int size)
{
	if ( size < 1)
	{
		printf("no arguments entered\n");
		return;
	}
	printf("Printing Arguments:\n");
	int i;
	for (i = 0; i < size; i++)
	{
		printf("%d. %s\n", i+1, args[i]);
	}
}

void add(char** args, int size)
{
	int i;
	int sum = 0;
	int currentArg;
	char needle[] = "0x";
	for(i = 1; i < size; i++)
	{
		if ( strstr(args[i], needle) != NULL)
		{
			//call hexconverter
			currentArg = hexConverter(args[i]);
		}
		else {
			currentArg = atoi(args[i]);
		}
		sum += currentArg;
	}
	printf("The sum is %d\n", sum);	
}

int hexConverter(char* argToConvert)
{
	int size = 0;
	int i = 0;
	double position = 0;
	int decimal = 0;
	int digit = 0;
	//figure out size of arg
	for ( i = 0; argToConvert[i] != '\0'; i++)
	{
		size++;
	}
	
	for ( i = size -1; i >= 2; i--)
	{
		digit = (int) (argToConvert[i] - '0');
		decimal += (int) (pow(16, position) * digit);
		position += 1;
	}
	
    return(decimal);
}

void backgroundCommand(char** args)
{
	int status = 1;
	int execReturn = 0;
	pid_t pid = fork();
	
	//parent
	if(pid > 0)
	{
		if ( waitpid( 0, &status, WNOHANG) == -1 && errno != ECHILD)
		{
			printf("wait failed in backgroundCommand function\n");
			printf("Error: %s", strerror(errno));
			exit(0);
		}
	}
	//child
	else if (pid == 0)
	{
		execReturn = execvp(args[0],args);
		if (execReturn == -1 )
		{
			printf("Command failed\n");
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}
	}
	else
	{
		printf("Error forking\n");
		printf("Error: %s \n", strerror(errno));
		exit(0);
	}
}

void foregroundCommand(char** args)
{
	int status = 1;
	int execReturn = 0;

	pid_t pid = fork();
	
	//parent
	if(pid > 0)
	{
		
		if (wait(&status) == -1 || WIFEXITED(status) != 1)
		{
			printf("Child did not terminate normally %s \n", WIFEXITED(status));
			printf("Error: %s \n", strerror(errno));
			printf("exiting the program.\n");
			exit(0);
		}
	}
	//child
	else if ( pid == 0) 
	{
		execReturn = execvp(args[0],args);
		if (execReturn == -1 )
		{
			printf("Command failed\n");
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}
	}
	else
	{
		printf("Errror forking\n");
		printf("Error: %s \n", strerror(errno));
		exit(0);
	}
	

}
	

//Loop through arguments to search for the '>' character
int isOutputRedirect (char **args, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)
	{
		if ( strcmp(args[i],">") == 0 )
		{
			return i;
		}
	}
	return 0;
}

//Loop through arguments to search for the '<' character
int isInputRedirect ( char**args, int size)
{
	int i = 0;
	for ( i = 0; i< size; i++)
	{
		if( strcmp(args[i], "<") == 0)
		{
			return i;
		}
	}
	return 0;
}

//function can redirect output to a file. Also it can redirect
// input from a file.
void redirect(char** args, int indexOfOut, int indexOfIn)
{
	pid_t pid = fork(); 
	
		
	//parent
	if (pid > 0)
	{	
		int status = 0;
		if ( wait(&status) == -1 || WIFEXITED(status) != 1)
		{
			printf("Child did not terminate normally %s \n", WIFEXITED(status));
			printf("Error: %s \n", strerror(errno));
			printf("exiting the program.\n");
			exit(0);
		}
	}
	//child
	else if (pid == 0)
	{
		int execReturn = 0;
		
		if (indexOfOut != 0)
		{
			char* outFile = args[indexOfOut+1];
			args[indexOfOut] = NULL;
			FILE *out;
			out = freopen(outFile,"w+", stdout);
			//error check freopen
			if (out == NULL)
			{
				printf("Could not redirect output.\n");
				printf("Error: %s\n", strerror(errno));
				exit(0);
			}
		}

		if (indexOfIn != 0)
		{
			char* inFile = args[indexOfIn+1];
			args[indexOfIn] = NULL;
			FILE *in;
			in = freopen(inFile, "r", stdin);
			if (in == NULL)
			{
				printf("Could not redirect input.\n");
				printf("Error: %s\n", strerror(errno));
				exit(0);
			}
		}
		
		execReturn = execvp(args[0],args);
		if (execReturn == -1)			
		{
			printf("Command failed\n");
			printf("Error: %s\n", strerror(errno));
			exit(0);
		}
		
	}
	else
	{
		printf("Error forking.");
		printf("Error: %s\n", strerror(errno));
		exit(0);
	}

}


void reap(int signum)
{
	int status = 12;
	if ( waitpid( 0, &status, WNOHANG) == -1 && errno != ECHILD)
	{
		printf("failed to reap %s \n", strerror(errno));
	}
}

