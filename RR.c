#include "RR.h"

void Scheduler_RR(struct Process *ps, int n) {
	qsort((void *)ps, n, sizeof(struct Process), Cmp_ReadyTime);

	int job_l = 0, job_r = 0;
	int cur_job = 0;
	int RR_count = kRRRound;
	for (int now_time = 0; ; ++now_time) {
		if (ps[cur_job].running && ps[cur_job].t == 0) {
			ps[cur_job].running = 0;
			wait(0);
			RR_count = kRRRound;

			while (job_l < job_r && ps[job_l].t == 0) ++job_l;
			if (job_l > cur_job) cur_job = job_l;
			if (cur_job == n) exit(0);
		}

		while (job_r < n && ps[job_r].s <= now_time) {
			StartProcess(&ps[job_r]);
			++job_r;
		}

		if (RR_count == 0) {
			SetPriority(ps[cur_job].pid, 1);
			ps[cur_job].running = 0;
			RR_count = kRRRound;

			do {
				++cur_job;
				if (cur_job >= job_r) cur_job = job_l;
			} while (ps[cur_job].t == 0);
		}

		if (job_l <= cur_job && cur_job < job_r &&
				ps[cur_job].spawned && ps[cur_job].t > 0) {
			SetPriority(ps[cur_job].pid, 99);
			ps[cur_job].running = 1;
		}

		TIME_UNIT;

		if (ps[cur_job].running) {
			assert(ps[cur_job].spawned);
			--ps[cur_job].t;
			--RR_count;
		}
	}
}

