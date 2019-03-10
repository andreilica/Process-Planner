build:planificator

planificator: main.o queue.o
	gcc -Wall main.o queue.o -o planificator

main.o: main.c
	gcc -Wall -c main.c

queue.o: queue.c
	gcc -Wall -c queue.c

clean:
	rm -rf *.o planificator