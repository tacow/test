#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <deque>

using namespace std;

void* PushbackThread(void* ptr) {
	deque<int>* queue = (deque<int>*)ptr;

	const int PUSHBACK_TIMES = 1000;
    for(int i = 0; i < PUSHBACK_TIMES; ++i) {
		queue->push_back(i);
    }
    return NULL;
}

void Test1() {
	const int PUSHBACK_THREAD_COUNT = 10;
    pthread_t pushbackThreads[PUSHBACK_THREAD_COUNT];

	deque<int> queue;
    for (int i = 0; i < PUSHBACK_THREAD_COUNT; ++i)
        pthread_create(&pushbackThreads[i], NULL, PushbackThread, &queue);

    for (int i = 0; i < PUSHBACK_THREAD_COUNT; ++i)
        pthread_join(pushbackThreads[i], NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage:\n");
        printf("    %s [case id](1)\n", argv[0]);
        return 0;
    }

    int caseID = atoi(argv[1]);
    switch (caseID) {
    case 1:
        Test1();
        break;
    }
    return 0;
}

