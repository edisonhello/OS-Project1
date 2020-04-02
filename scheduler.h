#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define _GNU_SOURCE

#include "util.h"

#include "FIFO.h"
#include "RR.h"

enum Policy GetPolicy();

void Scheduler_SJF(struct Process *ps, int n);
void Scheduler_PSJF(struct Process *ps, int n);

#endif // _SCHEDULER_H_
