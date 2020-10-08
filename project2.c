# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "record.h"




int main (int argc, char **argv)
{	

	// Opening files "grades.bin" if it does not exist we are creating it. Only user can read and write
	int fd = open("grades.bin", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);

	if(fd == -1 )
	{
		perror("Unable to open file");
		exit(-1);
	};

	
	while(fgets(inBuffer,BUFFERSIZE, stdin))
	{
		tokens[0]  = strtok(inBuffer,SEPERATORS);
		if(tokens[0] == NULL)
		{
				fprintf(stderr, "No Arguments\n");
		}
		else
		{
				int size = 1;
				for(int i = 0;tokens[i];++1){
						tokens[i+1] = strtok(NULL, SEPERATORS);
						size++;
				}
		}

	}

	 char command = tokens[0][0];

	
	return (0);
}
