CC = gcc
CFLAGS = -Wall -Wextra -g

# Objetivos
all: broker worker

# Reglas de compilación
broker: broker.o fbroker.o
	$(CC) $(CFLAGS) -o broker broker.o fbroker.o

worker: worker.o fworker.o
	$(CC) $(CFLAGS) -o worker worker.o fworker.o

# Reglas de dependencias
broker.o: broker.c fbroker.h
	$(CC) $(CFLAGS) -c broker.c

worker.o: worker.c fworker.h
	$(CC) $(CFLAGS) -c worker.c

fbroker.o: fbroker.c fbroker.h
	$(CC) $(CFLAGS) -c fbroker.c

fworker.o: fworker.c fworker.h
	$(CC) $(CFLAGS) -c fworker.c

# Regla para limpiar los archivos objeto y los ejecutables
clean:
	rm -f broker worker *.o

