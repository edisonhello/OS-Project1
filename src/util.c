#include "util.h"

int Cmp_ReadyTime(const void *_a, const void *_b) {
	const struct Process *a = (const struct Process *)_a;
	const struct Process *b = (const struct Process *)_b;

	if (a->s < b->s) return -1;
	if (a->s > b->s) return 1;
	if (a->_oi < b->_oi) return -1;
	if (a->_oi > b->_oi) return 1;
	return 0;
}

// void SetSignalHandler(void (*handler)(int)) {
// 	struct sigaction act;
// 
// 	act.sa_handler = handler;
// 	sigemptyset(&act.sa_mask);
// 	act.sa_flags = 0;
// 
// 	if (sigaction(SIGCHLD, &act, 0)) {
// 		fprintf(stderr, "sigaction error\n");
// 		exit(1);
// 	}
// }

void SetProcessCPU(pid_t pid, int id) {
	if (id >= CPU_SETSIZE) {
		fprintf(stderr, "SetProcessCPU get id too big: id = %d\n", id);
		exit(1);
	}

	cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(id, &cpu);

	if (sched_setaffinity(pid, sizeof(cpu), &cpu)) {
		fprintf(stderr, "sched_setaffinity error\n");
		exit(1);
	}
}

void SetPriority(pid_t pid, int pri) {
	struct sched_param para;
	para.sched_priority = pri;

	if (sched_setscheduler(pid, SCHED_FIFO, &para)) {
		fprintf(stderr, "sched_setscheduler error\n");
		exit(1);
	}
}

void StartProcess(struct Process *p) {
	pid_t pid = fork();

	if (pid < 0) {
		fprintf(stderr, "fork error\n");
		exit(1);
	}

	if (pid == 0) {
		struct timespec start, end;
		syscall(333, &start);
		for (int i = 0; i < p->t; ++i) 
			TIME_UNIT;
		syscall(333, &end);
		syscall(334, getpid(), start.tv_sec, start.tv_nsec, end.tv_sec, end.tv_nsec);
		exit(0);
	} else { 
		SetProcessCPU(pid, 1);
		SetPriority(pid, 1);
		p->pid = pid;
		p->spawned = 1;
		p->running = 0;

		printf("%s %d\n", p->name, (int)p->pid);

	}
}
