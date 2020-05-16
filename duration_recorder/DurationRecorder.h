#ifndef DURATION_RECORDER
#define DURATION_RECORDER

#include <time.h>
#include <string>
#include <map>

using namespace std;

class DurationRecorder {
public:
    DurationRecorder();
    ~DurationRecorder();

    void Start();
    void Record(const string& state);
    void Finish();

    string Dump();

private:
    string lastState_;
    struct timespec lastTS_;
    map<string, long> durationStat_;
};

#endif

