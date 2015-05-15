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

// interval应能够被3600整除
void ClockSleep(time_t interval, time_t* lastTime) {
    struct timeval now;
    gettimeofday(&now, NULL);

    time_t nextTime;
    if (now.tv_sec % interval == 0 && now.tv_usec == 0)
        nextTime = now.tv_sec;
    else
        nextTime = (now.tv_sec / interval + 1) * interval;

    if (*lastTime != 0) {
        time_t nextTime2 = *lastTime + interval;
        if (nextTime2 > nextTime)
            nextTime = nextTime2;
    }

    struct timeval nextTimeval;
    nextTimeval.tv_sec = nextTime;
    nextTimeval.tv_usec = 0;

    struct timeval waitTime;
    TimevalDiff(&nextTimeval, &now, &waitTime);
    select(0, NULL, NULL, NULL, &waitTime);
    *lastTime = nextTime;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [interval]\n", argv[0]);
        return 0;
    }
    time_t interval = atoi(argv[1]);

    time_t lastTime = 0;
    int i;
    for(i = 0; i < 10; ++i) {
        ClockSleep(interval, &lastTime);
        printf("%d\n", i);
    }
    return 0;
}

