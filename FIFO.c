#include "FIFO.h"

void Scheduler_FIFO(struct Process *ps, int n) {
	int cur_job = 0, runnable_job = 0;
	for (int now_time = 0; ; ++now_time) {
		if (ps[cur_job].running && ps[cur_job].t == 0) {
			ps[cur_job].running = 0;
			wait(0);
			fprintf(stderr, "Job %d done\n", cur_job);

			if (cur_job == n - 1) exit(0);

			++cur_job;
		}

		while (runnable_job < n && ps[runnable_job].s <= now_time) {
			StartProcess(&ps[runnable_job]);
			++runnable_job;
		}

		if (ps[cur_job].spawned && !ps[cur_job].running) {
			SetPriority(ps[cur_job].pid, 99);
			ps[cur_job].running = 1;
		}

		TIME_UNIT;

		if (ps[cur_job].running) {
			--ps[cur_job].t;
		}
	}
}

