#ifndef _UTIL_H_
#define _UTIL_H_

#define _GNU_SOURCE

#include <assert.h>
#include <sched.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#define kProcessNameLength 31

#define TIME_UNIT { volatile unsigned long i; for (i = 0; i < 1000000UL; i++); }

enum Policy { FIFO, RR, SJF, PSJF };

struct Process {
	char name[kProcessNameLength + 1];
	int s; // ready time
	int t; // execution time
	int spawned; // process is spawned
	int running; // process is running
	pid_t pid; // process pid
	int _oi; // original order as second key when sorting by ready time
};

int Cmp_ReadyTime(const void *_a, const void *_b);

void SetProcessCPU(pid_t pid, int id);
void SetPriority(pid_t pid, int pri);
void StartProcess(struct Process *p);

#endif // _UTIL_H_
