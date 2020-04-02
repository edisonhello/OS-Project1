#include "scheduler.h"

enum Policy GetPolicy() {
	char str[10];
	memset(str, 0, 10);
	scanf("%s", str);

	if (strcmp(str, "FIFO") == 0) return FIFO;
	if (strcmp(str, "RR") == 0) return RR;
	if (strcmp(str, "SJF") == 0) return SJF;
	if (strcmp(str, "PSJF") == 0) return PSJF;

	fprintf(stderr, "Unknown schedule policy: %s\n", str);
	exit(1);
}

void Scheduler_SJF(struct Process *ps, int n) {

}

void Scheduler_PSJF(struct Process *ps, int n) {

}

int main() {
	enum Policy policy = GetPolicy();

	int n;
	scanf("%d", &n);

	struct Process ps[n];
	for (int i = 0; i < n; ++i) 
		scanf("%s%d%d", ps[i].name, &ps[i].s, &ps[i].t);

	SetProcessCPU(getpid(), 0);

	if (policy == FIFO) Scheduler_FIFO(ps, n);
	else if (policy == RR) Scheduler_RR(ps, n);
	else if (policy == SJF) Scheduler_SJF(ps, n);
	else if (policy == PSJF) Scheduler_PSJF(ps, n);
	else {
		fprintf(stderr, "Unknown policy type = %d\n", policy);
		exit(1);
	}
}
