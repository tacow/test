#include "DurationRecorder.h"
#include <stdio.h>
#include <unistd.h>

int main() {
    DurationRecorder recorder;
    for (int i = 0; i < 100; ++i) {
        recorder.Start();
        usleep(300);
        recorder.Record("1_state1");
        usleep(200);
        recorder.Record("2_state2");
        usleep(500);
        recorder.Record("3_state3");
        usleep(800);
        recorder.Record("4_state4");
        usleep(600);
        recorder.Record("5_state5");
        usleep(700);
        recorder.Finish();
    }
    string statStr = recorder.Dump();
    printf("%s", statStr.c_str());
    return 0;
}


