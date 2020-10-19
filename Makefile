all: create

create:
	gcc -o project2 project2.c
clean:
	rm -rf *.o project2
