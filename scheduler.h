#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define _GNU_SOURCE

#include "util.h"

#include "FIFO.h"
#include "RR.h"
#include "SJF.h"
#include "PSJF.h"

enum Policy GetPolicy();

#endif // _SCHEDULER_H_
