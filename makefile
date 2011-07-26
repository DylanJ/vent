all: main

main: objects
	gcc -pthread -lventrilo3 *.o -o vent

objects:
	gcc -Wall -c *.c

clean:
	rm *.o
	rm vent
