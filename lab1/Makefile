CC = gcc
CFLAGS = -Wall

lab1: lab1.o funciones.o
	$(CC) $(CFLAGS) -o lab1 lab1.o funciones.o

lab1.o: lab1.c funciones.h
	$(CC) $(CFLAGS) -c lab1.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

clean:
	rm -f lab1 lab1.o funciones.o

