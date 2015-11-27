#include "GPerfUtil.h"
#include <string.h>
#include <google/heap-profiler.h>
#include <google/profiler.h>

bool g_heapProfilerStarted = false;
bool g_cpuProfilerStarted = false;

int  g_heapProfilerSignal = 0;
int  g_cpuProfilerSignal = 0;

void SignalHandler(int signum) {
    if (signum == g_heapProfilerSignal) {
        if (!g_heapProfilerStarted) {
            HeapProfilerStart("profile");
            g_heapProfilerStarted = true;
        }
        HeapProfilerDump("user triggered");
    } else if (signum == g_cpuProfilerSignal) {
        if (!g_cpuProfilerStarted) {
            ProfilerStart("cpuprofile");
            g_cpuProfilerStarted = true;
        } else {
            ProfilerStop();
            g_cpuProfilerStarted = false;
        }
    }
}

void SetupGPerf(int heapProfilerSignal, int cpuProfilerSignal) {
    g_heapProfilerSignal = heapProfilerSignal;
    g_cpuProfilerSignal = cpuProfilerSignal;

    struct sigaction act;
    memset(&act, 0, sizeof(act));
    act.sa_handler = SignalHandler;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGUSR1);
    sigaddset(&act.sa_mask, SIGUSR2);
    sigaction(SIGUSR1, &act, NULL);
    sigaction(SIGUSR2, &act, NULL);
}

