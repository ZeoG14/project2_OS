all: create

create:
	gcc -Wall -g -o project2 project2.c
	
.PHONY clean:
	rm -rf *o project2
