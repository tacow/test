#include "timer.h"

void Timer::Start() {
    gettimeofday(&m_startTime, NULL);
}

void Timer::Stop() {
    gettimeofday(&m_stopTime, NULL);
}

long Timer::GetMicroseconds() {
    return (m_stopTime.tv_sec - m_startTime.tv_sec) * 1000000 + (m_stopTime.tv_usec - m_startTime.tv_usec);
}

