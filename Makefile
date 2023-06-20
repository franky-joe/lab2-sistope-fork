CC = gcc
CFLAGS = -g -Wall
OBJS = fworker.o fbroker.o lab2.o funciones.o worker.o broker.o

lab2: $(OBJS)
	$(CC) $(CFLAGS) -o lab2 $(OBJS)

lab2.o: lab2.c fworker.h fbroker.h funciones.h worker.h broker.h
	$(CC) $(CFLAGS) -c lab2.c

fworker.o: fworker.c fworker.h funciones.h
	$(CC) $(CFLAGS) -c fworker.c

fbroker.o: fbroker.c fbroker.h funciones.h
	$(CC) $(CFLAGS) -c fbroker.c

funciones.o: funciones.c funciones.h
	$(CC) $(CFLAGS) -c funciones.c

worker.o: worker.c worker.h
	$(CC) $(CFLAGS) -c worker.c

broker.o: broker.c broker.h
	$(CC) $(CFLAGS) -c broker.c

clean:
	rm -f $(OBJS) lab2

