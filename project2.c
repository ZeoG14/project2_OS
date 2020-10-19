# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include "record.h"
# include <ctype.h>

const int MAXSIZE = 23;
const int SIZE_TYPE_NAMES = 4;

void append(char**, int );
void score(char**, int);
void valid(char**, int);
void invalid(char**, int);
void text(int fd);



int main (int argc, char** argv)
{	
		// Opening files "grades.bin" if it does not exist we are creating it. Only user can read and write
		 int fd = open("grades.bin", O_RDWR| O_CREAT, 0777);

		// Writing error to stderr if file could not be opened
		if(fd == -1 )
		{
				perror("Unable to open file");
				exit(-1);
		};

		char* command = argv[1];

		// This will contain the command "append, valid, score etc...."
		//My first approach will be to use if and else if statements since we can't use string with switch statement
		
    		if(argc == 1){
				fprintf(stderr, "No Arguments");

				exit(-1);
		}

		// if the command given is append
		if(strcmp("append",command) == 0 ){
			if(argv[2] == NULL){
				fprintf(stderr,"Invalid Arguments\n");
				exit(-1);
			}

			if(argc != 6){
				fprintf(stderr, "Invalid Argument\n");
				exit(-1);
			}
				append(argv,fd);
		}

		// else if command given is score
		else if(strcmp("score", command) == 0){
			if(argv[2] == NULL){
				fprintf(stderr,"Invalid Arguments\n");
				exit(-1);
			}
				score(argv, fd);
		}

		//else if command given is valid
		else if(strcmp("valid", command)==0){
			if(argv[2] == NULL){
				fprintf(stderr,"Invalid Arguments\n");
				exit(-1);
			}
			valid(argv,fd);
		}


		else if(strcmp("invalid", command) == 0 ){
			if(argv[2] == NULL){
				fprintf(stderr,"Invalid Arguments\n");
				exit(-1);
			}
			printf("In else if\n");
			invalid(argv,fd);
		}

		else if(strcmp("text", command) == 0){
			text(fd);
		}



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
	       	for(int i = 0; i < MAXSIZE; ++i ){hwName[i] = '\0';}	
		if(args[2] == NULL) exit(-1);
		if(sscanf(args[2], "%23[^\n]",hwName) != 1){
				fprintf(stderr,"Invalid hwName");
				exit(-1);
		}
		//Reading in the HW Type
		char hwType[MAXSIZE];
		if(args[3] == NULL) exit(-1);
		if(sscanf(args[3], "%s[^\n]", hwType) != 1){
				fprintf(stderr,"Ivalid HW Type");
				exit(-1);
		}
		//Reading int the Max possible score 
		float maxScore;
		if(args[4] == NULL) exit(-1);
		if(sscanf(args[4], "%f", &maxScore) != 1){
				fprintf(stderr,"Invalid maxScore");
				exit(-1);
		}
		//Reading in the actual score 
		float actScore;

		if(args[5] == NULL) exit(-1);
		if(sscanf(args[5], "%f", &actScore) != 1){
				fprintf(stderr, "Invalid score");
				exit(-1);
		}


		//ensuring it is a proper string by adding the terminating character to the end
		hwName[NAME_SIZE - 1] = '\0';


		for(int i = 0; i < strlen(hwType); ++i){
			if(isalpha(hwType[i]) == 0){
				fprintf(stderr, "Invalid Name\n");
				exit(-1);
			}
		}
		
		// declaring first instance of AssignmentRecord from Record.h
		AssignmentRecord ar;
		//creating a temp buffer to make sure all values are 0;
		unsigned char* b[sizeof(AssignmentRecord)];
		for(int i=0; i<sizeof(AssignmentRecord); ++i){
			b[i] = 0x00;		}
		memcpy(&ar, b, sizeof(AssignmentRecord));
		//By default valid should be 1 which in this case is true
		ar.valid = 1;
		//using strcpy() to assing hwName to ar.name
		strcpy(ar.name, hwName); 
		//Setting the read-in maxScore to our record
		ar.max_score = maxScore;
		// Setting the actual score to our record
		ar.score=actScore;
		
		int i;
		//Iterating through type_name which is an array in Record.h
		for(i = 0; i < SIZE_TYPE_NAMES ;++i ){
				// iterating until we find the int position of the hwType
				if(strcmp(type_names[i],hwType) == 0){
						//assigning int position to ar.type
						ar.type = i;
						//since it should only match once breaking immediately
						break; 
				}
		}
		//If it was not found
		if(i >= SIZE_TYPE_NAMES)
		{
			fprintf(stderr, "INV TYPE: \n");
			exit(-1);
		}
		int size = sizeof(AssignmentRecord);
		if(lseek(fd, 0, SEEK_END) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//writing out our ar(record) to our file
		if(write(fd, &ar, size) != size){
				perror("Write");
				exit(-1);
		}

		/*
				10/13/20 - This method appears to be functioning as intended but further testing will be needed.
		   */
		
}



void score(char** args,int fd){

		//Reading in the index of the score to be changed 
		int index;
		if(sscanf(args[2], "%d", &index) != 1 ){
				fprintf(stderr,"Invalid Index");
				exit(-1);
		} 
		//Reading in the new score value;
		float score;
		if(sscanf(args[3], "%f", &score) != 1){
				fprintf(stderr, "Invalid Score");
				exit(-1);
		}
				

		int size =  sizeof(AssignmentRecord);
		//Getting the offset by multiplying by the size of Assignment Record
		int byteLoc = index * size;
		//Created the variable so the data can be read into it
		AssignmentRecord ar;
		

		//Getting the amount of bytes currently in the file
		off_t eof = lseek(fd,0,SEEK_END);
		//byteLoc is the byte location of the index, if this location if past the EOF than return an error.
		if(byteLoc > eof){
			fprintf(stderr, "Invalid Index\n");
			exit(-1);
		}

		//setting the current file location to the byteLocation of the valid index
		if(lseek(fd, byteLoc, SEEK_SET) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//Reading in the data at the index and assigning it to ar
		if(read(fd,&ar, size) == -1){
				perror("read error");
				exit(-1);
		}

		//Changing the value of score
		ar.score = score;

		if(lseek(fd, byteLoc, SEEK_SET) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//Writing the modified Assignment Record back to the file.
		if(write(fd,&ar,size) != size){
				perror("write error: ");
				exit(-1);
		}

		/*
				Function appears to be functioning correctly. More testing may be needed.
		   */

}






void valid(char** args,int fd){

		//Reading in the index of the Assignment Record to be changed 
		int index;
		if(sscanf(args[2], "%d", &index) != 1 ){
				fprintf(stderr,"Invalid Index");
				exit(-1);
		} 
				
		//byteSize of AssignmentRecord
		int size =  sizeof(AssignmentRecord);

		//Getting the offset by multiplying by the size of Assignment Record
		int byteLoc = index * size;

		//Created the variable so the data can be read into it
		AssignmentRecord ar;

		//Getting the amount of bytes currently in the file
		off_t eof = lseek(fd,0,SEEK_END);
		
		//byteLoc is the byte location of the index, if this location if past the EOF then return an error.
		if(byteLoc > eof){
			fprintf(stderr, "Invalid Index\n");
			exit(-1);
		}

		//setting the current file location to the byteLocation of the valid index
		if(lseek(fd, byteLoc, SEEK_SET) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//Reading in the data at the index and assigning it to ar
		if(read(fd,&ar, size) == -1){
				perror("read error");
				exit(-1);
		}
		
		//Changing the value of score
		ar.valid = 1;

		if(lseek(fd, byteLoc, SEEK_SET) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//Writing the modified Assignment Record back to the file.
		if(write(fd,&ar,size) != size){
				perror("write error: ");
				exit(-1);
		}

		/*
				Function appears to be functioning correctly. More testing may be needed.
		   */

}



void invalid(char** args,int fd){

		//Reading in the index of the Assignment Record to be changed 
		int index;
		if(sscanf(args[2], "%d", &index) != 1 ){
				fprintf(stderr,"Invalid Index");
				exit(-1);
		} 
				
		//byteSize of AssignmentRecord
		int size =  sizeof(AssignmentRecord);

		//Getting the offset by multiplying by the size of Assignment Record
		int byteLoc = index * size;

		//Created the variable so the data can be read into it
		AssignmentRecord ar;
		

		//Getting the amount of bytes currently in the file
		off_t eof = lseek(fd,0,SEEK_END);
		
		//byteLoc is the byte location of the index, if this location if past the EOF then return an error.
		if(byteLoc > eof){
			fprintf(stderr, "Invalid Index\n");
			exit(-1);
		}

		//setting the current file location to the byteLocation of the valid index
		if(lseek(fd, byteLoc, SEEK_SET) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//Reading in the data at the index and assigning it to ar
		if(read(fd,&ar, size) == -1){
				perror("read error");
				exit(-1);
		}

		
		//Changing the value of score to reflect an invalid score
		ar.valid = 0;

		if(lseek(fd, byteLoc, SEEK_SET) == -1){
				perror("lseek error: ");
				exit(-1);
		}

		//Writing the modified Assignment Record back to the file.
		if(write(fd,&ar,size) != size){
				perror("write error: ");
				exit(-1);
		}

		/*
				Function appears to be functioning correctly. More testing may be needed.
		   */

}



void text(int fd){
	
	AssignmentRecord ar;
	
	int byteLoc = 0 * sizeof(AssignmentRecord);
	int size =  sizeof(AssignmentRecord);

	if(lseek(fd,byteLoc, SEEK_CUR) == -1)exit(-1);

	if(read(fd,&ar,size)  == -1){
		exit(-1);
	}
	printf("%10d %10s %10d %10f %10f\n", ar.valid, ar.name, ar.type, ar.max_score, ar.score);


	
	byteLoc = 1 * sizeof(AssignmentRecord);
	size =  sizeof(AssignmentRecord);

	if(lseek(fd,byteLoc, SEEK_CUR) == -1)exit(-1);

	if(read(fd,&ar,size)  == -1){
		exit(-1);
	}
	printf("%10d %10s %10d %10f %10f\n", ar.valid, ar.name, ar.type, ar.max_score, ar.score);


	 
	byteLoc = 2  * sizeof(AssignmentRecord);
	size =  sizeof(AssignmentRecord);

	if(lseek(fd,byteLoc, SEEK_CUR) == -1)exit(-1);

	if(read(fd,&ar,size)  == -1){
		exit(-1);
	}
	printf("%10d %10s %10d %10f %10f\n", ar.valid, ar.name, ar.type, ar.max_score, ar.score);
}

	
