#include "GPerfUtil.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <deque>

using namespace std;

int main() {
    SetupGPerf();

    while(true) {
        char* buf = new char[10 * 1024 * 1024];
        memset(buf, 0, 10 * 1024 * 1024);

        deque<int> di;
        for(int i = 0; i < 10000; ++i)
            di.push_back(i);

        fputs("#", stdout);
        fflush(stdout);
        sleep(1);
    }
    return 0;
}

