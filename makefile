all: main

main: objects
	gcc -pthread -lventrilo3 *.o -o vent

objects:
	gcc -Wall -c *.c

git:
	git push -u origin master
clean:
	rm *.o
	rm vent
