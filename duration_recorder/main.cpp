#include "DurationRecorder.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    DurationRecorder recorder;
    for (int i = 0; i < 10; ++i) {
        recorder.Start();
        usleep(3000);
        recorder.Record("1_state1");
        usleep(2000);
        recorder.Record("2_state2");
        usleep(5000);
        recorder.Record("3_state3");
        usleep(8000);
        recorder.Record("4_state4");
        usleep(6000);
        recorder.Record("5_state5");
        usleep(7000);
        recorder.Finish();
    }
    string statStr = recorder.Dump();
    printf("%s", statStr.c_str());
    return 0;
}


