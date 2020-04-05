#include <unistd.h>
#include "mylog.h"

int main() {
    MyLog("abc %d", 123);
    sleep(2);
    MyLog("def %d", 456);
    return 0;
}

