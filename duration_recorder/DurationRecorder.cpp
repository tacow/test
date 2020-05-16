#include "DurationRecorder.h"
#include <string.h>
#include <sstream>

using namespace std;

long TimeDiff(struct timespec* from, struct timespec* to) {
    return (to->tv_sec - from->tv_sec) * 1000000 + (to->tv_nsec - from->tv_nsec) / 1000;
}

DurationRecorder::DurationRecorder() {
    memset(&lastTS_, 0, sizeof(lastTS_));
}

DurationRecorder::~DurationRecorder() {
}

void DurationRecorder::Start() {
    lastState_ = "0_start";
    memset(&lastTS_, 0, sizeof(lastTS_));
    clock_gettime(CLOCK_MONOTONIC, &lastTS_);
}

void DurationRecorder::Record(const string& state) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long duration = TimeDiff(&lastTS_, &ts);
    string durationName = lastState_ + " ~ " + state;
    durationStat_[durationName] += duration;

    lastState_ = state;
    memset(&lastTS_, 0, sizeof(lastTS_));
    clock_gettime(CLOCK_MONOTONIC, &lastTS_);
}

void DurationRecorder::Finish() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    long duration = TimeDiff(&lastTS_, &ts);
    string durationName = lastState_ + " ~ finish";
    durationStat_[durationName] += duration;

    lastState_.clear();
    memset(&lastTS_, 0, sizeof(lastTS_));
}

string DurationRecorder::Dump() {
    ostringstream oss;
    for (map<string, long>::iterator it = durationStat_.begin(); it != durationStat_.end(); ++it) {
        const string& durationName = it->first;
        long totalDuration = it->second;
        oss << durationName << ": " << totalDuration << endl;
    }
    return oss.str();
}

