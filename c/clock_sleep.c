#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// timeval比较
// tv1 > tv2时返回1
// tv1 = tv2时返回0
// tv1 < tv2时返回-1
int TimevalComp(struct timeval* tv1, struct timeval* tv2) {
    if (tv1->tv_sec > tv2->tv_sec)
        return 1;
    if (tv1->tv_sec < tv2->tv_sec)
        return -1;
    if (tv1->tv_usec > tv2->tv_usec)
        return 1;
    if (tv1->tv_usec < tv2->tv_usec)
        return -1;
    return 0;
}

// timeval求差，tv1应大于等于tv2
void TimevalDiff(struct timeval* tv1, struct timeval* tv2, struct timeval* diff) {
    if (tv1->tv_usec >= tv2->tv_usec) {
        diff->tv_sec = tv1->tv_sec - tv2->tv_sec;
        diff->tv_usec = tv1->tv_usec - tv2->tv_usec;
    } else {
        diff->tv_sec = tv1->tv_sec - 1 - tv2->tv_sec;
        diff->tv_usec = tv1->tv_usec + 1000000 - tv2->tv_usec;
    }
}

// 休眠到时钟周期
// interval为周期，单位秒，数值应能够被3600（1小时）整除
// offset为偏移，单位秒，数值应小于interval，若无需偏移则传0
// lastTime为内部状态变量，每次需传入，且第一次请求时设为0
void ClockSleep(time_t interval, time_t offset, time_t* lastTime) {
    struct timeval now;
    gettimeofday(&now, NULL);

    struct timeval nextTime;
    nextTime.tv_sec = now.tv_sec / interval * interval + offset;
    nextTime.tv_usec = 0;

    if (TimevalComp(&now, &nextTime) > 0)
        nextTime.tv_sec += interval;

    if (*lastTime != 0) {
        time_t nextTime2 = *lastTime + interval;
        if (nextTime2 > nextTime.tv_sec)
            nextTime.tv_sec = nextTime2;
    }

    struct timeval waitTime;
    TimevalDiff(&nextTime, &now, &waitTime);
    select(0, NULL, NULL, NULL, &waitTime);
    *lastTime = nextTime.tv_sec;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage:\n");
        printf("    %s [interval] [offset]\n", argv[0]);
        return 0;
    }
    time_t interval = atoi(argv[1]);
    time_t offset   = atoi(argv[2]);

    time_t lastTime = 0;
    int i;
    for(i = 0; i < 10; ++i) {
        ClockSleep(interval, offset, &lastTime);
        printf("%d\n", i);
    }
    return 0;
}

