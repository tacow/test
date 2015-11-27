#ifndef GPERF_UTIL_H
#define GPERF_UTIL_H

#include <signal.h>

void SetupGPerf(int heapProfilerSignal = SIGUSR1, int cpuProfilerSignal = SIGUSR2);

#endif

