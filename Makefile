
all:
	make -C src/
	cp src/scheduler scheduler

clean:
	make clean -C src/
	rm  scheduler
