CC = gcc
CFLAGS = -g -Wall
OBJS = fworker.o fbroker.o lab2.o funciones.o

lab2: $(OBJS)
	$(CC) $(CFLAGS) -o lab2 $(OBJS)

lab2.o: lab2.c fworker.h fbroker.h funciones.h
	$(CC) $(CFLAGS) -c lab2.c

fworker.o: fworker.c fworker.h funciones.h
	$(CC) $(CFLAGS) -c fworker.c

fbroker.o: fbroker.c fbroker.h funciones.h
	$(CC) $(CFLAGS) -c fbroker.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

clean:
	rm -f $(OBJS) lab2

