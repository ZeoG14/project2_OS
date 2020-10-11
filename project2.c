# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include "record.h"

const int BUFFERSIZE = 112;
const char SEPERATORS[] = " \t\n";
const int MAXSIZE = 20;

void append(int, char** );


int main (int argc, char** argv)
{	
     char inBuffer[BUFFERSIZE];
	 char* tokens[MAXSIZE];
     // Opening files "grades.bin" if it does not exist we are creating it. Only user can read and write
	 int fd = open("grades.bin", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	// Writing error to stderr if file could not be opened
	if(fd == -1 )
	{
		perror("Unable to open file");
		exit(-1);
	};

		char* command = argv[1];

		// This will contain the command "append, valid, score etc...."
		//My first approach will be to use if and else if statements since we can't use string with switch statement
		
		// if the command given is append
    	if(argc == 1){
				fprintf(stderr, "No Arguments");

				exit(-1);
		}

		if(strcmp("append",command) == 0 ){
				append(argc, argv);
		}


		else if(strcmp("score", command) == 0){
				printf("This will change the score");
		}



	return (0);
}



void append(int size, char** args){
		
		//Reading in the HW name
		char hwName[MAXSIZE]; 
		sscanf(args[2], "%[^\n]",hwName);
		//Reading in the HW Type
		char hwType[MAXSIZE];
		sscanf(args[3], "%[^\n]", hwType);
		//Reading int the Max possible score 
		float maxScore;
		sscanf(args[4], "%f", &maxScore);
		//Reading in the actual score 
		float actScore;
		if(sscanf(args[5], "%f", &actScore) != 1){
		perror("Wrong score type");
		}

		printf("%s %s  %.2f %.2f\n", hwName,hwType, maxScore, actScore);
}
