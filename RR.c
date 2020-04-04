#include "RR.h"

void Scheduler_RR(struct Process *ps, int n) {
	int runnable_job = 0;
	int RR_count = kRRRound;

	struct Queue *head = 0, *tail = 0;
	for (int now_time = 0; ; ++now_time) {
		if (head && head->p->t == 0) {
			head->p->running = 0;
			wait(0);
			RR_count = kRRRound;

			struct Queue *now = head;
			if (head->next) head = head->next;
			else head = tail = 0;

			free(now);

			if (!head && runnable_job == n) exit(0);
		}

		while (runnable_job < n && ps[runnable_job].s <= now_time) {
			StartProcess(&ps[runnable_job]);
			struct Queue *now = (struct Queue *)calloc(1, sizeof(struct Queue));
			now->p = &ps[runnable_job];

			if (!head) head = tail = now;
			else {
				tail->next = now;
				tail = now;
			}

			++runnable_job;
		}

		if (RR_count == 0) {
			SetPriority(head->p->pid, 1);
			head->p->running = 0;
			RR_count = kRRRound;

			if (head->next) {
				struct Queue *now = head;
				head = head->next;
				now->next = 0;
				tail->next = now;
				tail = now;
			}
		}

		if (head && !head->p->running) {
			SetPriority(head->p->pid, 99);
			head->p->running = 1;
		}

		TIME_UNIT;

		if (head) {
			--head->p->t;
			--RR_count;
		}
	}
}

