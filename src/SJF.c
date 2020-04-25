#include "SJF.h"

void Scheduler_SJF(struct Process *ps, int n) {
	int runnable_job = 0;
	struct Node *root = 0;
	struct Process *p = 0;
	for (int now_time = 0; ; ++now_time) {
		if (p && p->t == 0) {
			p->running = 0;
			wait(0);

			if (runnable_job == n && !root) return;
			p = 0;
		}

		while (runnable_job < n && ps[runnable_job].s <= now_time) {
			Meow_Insert(&root, ps[runnable_job].t, ps[runnable_job]._oi, &ps[runnable_job]);
			++runnable_job;
		}

		if (!p && root) {
			p = (struct Process *)Meow_PopBegin(&root);
			if (!p->spawned) StartProcess(p);
			SetPriority(p->pid, 99);
			p->running = 1;
		}

		TIME_UNIT;

		if (p) --p->t;
	}
}

