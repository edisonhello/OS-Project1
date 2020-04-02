CC := gcc
flags := -std=c11 -O3 -W -Wall -Wextra

all: scheduler

FIFO.o: FIFO.c FIFO.h
	$(CC) $(flags) FIFO.c -c -o FIFO.o

RR.o: RR.c RR.h
	$(CC) $(flags) RR.c -c -o RR.o

util.o: util.c util.h
	$(CC) $(flags) util.c -c -o util.o

scheduler: scheduler.c scheduler.h util.o FIFO.o RR.o
	$(CC) $(flags) scheduler.c util.o FIFO.o RR.o -o scheduler

clean:
	rm -rf scheduler util.o FIFO.o
