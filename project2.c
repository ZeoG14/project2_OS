# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include "record.h"


const int MAXSIZE = 20;
const int SIZE_TYPE_NAMES = 4;

void append(char**, int );
void score(char**);






int main (int argc, char** argv)
{	
		// Opening files "grades.bin" if it does not exist we are creating it. Only user can read and write
		 int fd = open("grades.bin", O_WRONLY|O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

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
				append(argv,fd);
		}


		else if(strcmp("score", command) == 0){
				score(argv);
		}

		/*

				ADD THE REST OF THE POSSIBLE DATABASE COMMANDS AND ALSO ENSURE THAT ONE OF THEM IS CALLED IF THERE IS NO MATCH THAN THE PROGRAM SHOULD EXIT

		   */





		if(close(fd) != 0){
				perror("Close error");
				exit(-1);
		}

	return (0);
}



void append(char** args, int fd){

		/*


				FOR THE STRINGS READING IN INTS OR FLOATS MAY NOT BE ACCEPTABLE THIS CODE MAY HAVE TO CHANGE LATER

		   */
		
		//Reading in the HW name
		char hwName[MAXSIZE]; 
		if(sscanf(args[2], "%23[^\n]",hwName) != 1){
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


		//ensuring it is a proper string by adding the terminating character to the end
		hwName[NAME_SIZE - 1] = 0;

		// declaring first instance of AssignmentRecord from Record.h
		AssignmentRecord ar;
		//By default valid should be 0 which in this case is true
		ar.valid = 0;
		//using strcpy() to assing hwName to ar.name
		strcpy(ar.name, hwName); 
		//Setting the read-in maxScore to our record
		ar.max_score = maxScore;
		// Setting the actual score to our record
		ar.score = actScore;
		

		//Iterating through type_name which is an array in Record.h
		for(int i = 0; i < SIZE_TYPE_NAMES ;++i ){
				// iterating until we find the int position of the hwType
				if(strcmp(type_names[i],hwType) == 0){
						//assigning int position to ar.type
						ar.type = i;
						//since it should only match once breaking immediately
						break; 
				}
		}

		/*

				TODO:
				
				fix offset since it is currently overwriting the previous record.
				should probably use lseek()
		   */
				
		//writing out our ar(record) to our file
		if(write(fd, &ar, sizeof(AssignmentRecord)) != sizeof(AssignmentRecord)){
				perror("Write");
				exit(-1);
		}
		
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
