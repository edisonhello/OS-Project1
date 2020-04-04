#ifndef _RR_H_
#define _RR_H_

#include "util.h"

#define kRRRound 500

struct Queue {
	struct Process *p;
	struct Queue *next;
};

void Scheduler_RR(struct Process *ps, int n);

#endif // _RR_H_
