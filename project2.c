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

void append(char** );
void score(char**);






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
				append(argv);
		}


		else if(strcmp("score", command) == 0){
				score(argv);
		}

		/*

				ADD THE REST OF THE POSSIBLE DATABASE COMMANDS AND ALSO ENSURE THAT ONE OF THEM IS CALLED IF THERE IS NO MATCH THAN THE PROGRAM SHOULD EXIT

		   */


	return (0);
}



void append(char** args){

		/*


				FOR THE STRINGS READING IN INTS OR FLOATS MAY NOT BE ACCEPTABLE THIS CODE MAY HAVE TO CHANGE LATER

		   */
		
		//Reading in the HW name
		char hwName[MAXSIZE]; 
		if(sscanf(args[2], "%[^\n]",hwName) != 1){
				fprintf(stderr,"Invalid hwName");
				exit(-1);
		}
		//Reading in the HW Type
		char hwType[MAXSIZE];
		if(sscanf(args[3], "%[^\n]", hwType) != 1){
				fprintf(stderr,"Ivalid HW Type");
				exit(-1);
		}
		//Reading int the Max possible score 
		float maxScore;
		if(sscanf(args[4], "%f", &maxScore)!= 1){
				fprintf(stderr,"Invalid maxScore");
				exit(-1);
		}
		//Reading in the actual score 
		float actScore;
		if(sscanf(args[5], "%f", &actScore) != 1){
				fprintf(stderr, "Invalid score");
				exit(-1);
		}

		printf("%s %s %.2f %.2f\n", hwName,hwType, maxScore, actScore);
}



void score(char** args){

		//Reading in the index of the score to be changed 
		int index;
		if(sscanf(args[2], "%d", &index) != 1 ){
				fprintf(stderr,"Invalid Index");
				exit(-1);
		} 

		float score;
		if(sscanf(args[3], "%f", &score) != 1){
				fprintf(stderr, "Invalid Score");
				exit(-1);
		}

		printf("%d %f", index, score);

}
