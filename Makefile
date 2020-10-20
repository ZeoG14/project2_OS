all: create fifo
fifo:
	mkfifo grades.html

create:
	gcc -Wall -c -g project2.c -o project2.o
	gcc project2.o -o project2

clean:
	rm -rf *o project2

