CC := gcc
flags := -std=c11 -O3 -W -Wall -Wextra -g

all: scheduler

util.o: util.c util.h
	$(CC) $(flags) util.c -c -o util.o

meow.o: meow.c meow.h
	$(CC) $(flags) meow.c -c -o meow.o

FIFO.o: FIFO.c FIFO.h
	$(CC) $(flags) FIFO.c -c -o FIFO.o

RR.o: RR.c RR.h
	$(CC) $(flags) RR.c -c -o RR.o

SJF.o: SJF.c SJF.h 
	$(CC) $(flags) SJF.c -c -o SJF.o

PSJF.o: PSJF.c PSJF.h 
	$(CC) $(flags) PSJF.c -c -o PSJF.o

scheduler: scheduler.c scheduler.h util.o meow.o FIFO.o RR.o SJF.o PSJF.o
	$(CC) $(flags) scheduler.c util.o meow.o FIFO.o RR.o SJF.o PSJF.o -o scheduler

clean:
	rm -rf scheduler util.o meow.o FIFO.o RR.o SJF.o PSJF.o
