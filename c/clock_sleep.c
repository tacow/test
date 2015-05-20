#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

// timeval�Ƚ�
// tv1 > tv2ʱ����1
// tv1 = tv2ʱ����0
// tv1 < tv2ʱ����-1
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

// timeval��tv1Ӧ���ڵ���tv2
void TimevalDiff(struct timeval* tv1, struct timeval* tv2, struct timeval* diff) {
    if (tv1->tv_usec >= tv2->tv_usec) {
        diff->tv_sec = tv1->tv_sec - tv2->tv_sec;
        diff->tv_usec = tv1->tv_usec - tv2->tv_usec;
    } else {
        diff->tv_sec = tv1->tv_sec - 1 - tv2->tv_sec;
        diff->tv_usec = tv1->tv_usec + 1000000 - tv2->tv_usec;
    }
}

// ���ߵ�ʱ������
// intervalΪ���ڣ���λ�룬��ֵӦ�ܹ���3600��1Сʱ������
// offsetΪƫ�ƣ���λ�룬��ֵӦС��interval��������ƫ����0
// lastTimeΪ�ڲ�״̬������ÿ���贫�룬�ҵ�һ������ʱ��Ϊ0
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

