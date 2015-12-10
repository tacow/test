#ifndef TIMER_H
#define TIMER_H

#include <stddef.h>
#include <sys/time.h>

class Timer {
public:
    void Start();
    void Stop();

    long GetMicroseconds();

public:
    timeval m_startTime;
    timeval m_stopTime;
};

#endif

