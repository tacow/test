#include <stdio.h>
#include <sys/time.h>
#include <list>
#include <vector>
#include <deque>
using namespace std;

#define LOOP_NUM 1000
#define OP_NUM   10000

long TimevalDiff(const timeval& tvStart, const timeval& tvEnd) {
    return (tvEnd.tv_sec - tvStart.tv_sec) * 1000000 + (tvEnd.tv_usec - tvStart.tv_usec);
}

template<typename Container>
void TestPush(const char* filename) {
    FILE* f = fopen(filename, "w");

    Container c;
    for(int i = 0; i < LOOP_NUM; ++i) {
        timeval tvStart;
        gettimeofday(&tvStart, NULL);

        for(int j = 0; j < OP_NUM; ++j)
            c.push_back(j);

        timeval tvEnd;
        gettimeofday(&tvEnd, NULL);

        long span = TimevalDiff(tvStart, tvEnd);
        fprintf(f, "%ld\n", span);
    }

    fclose(f);
}

template<typename Container>
void TestAccess(const char* filename) {
    FILE* f = fopen(filename, "w");

    Container c;
    for(int i = 0; i < OP_NUM; ++i)
        c.push_back(i);

    for(int i = 0; i < LOOP_NUM; ++i) {
        timeval tvStart;
        gettimeofday(&tvStart, NULL);

        int sum = 0;
        for(int j = 0; j < OP_NUM; ++j)
            sum += c[j];

        timeval tvEnd;
        gettimeofday(&tvEnd, NULL);

        long span = TimevalDiff(tvStart, tvEnd);
        fprintf(f, "%ld\n", span);
    }

    fclose(f);
}

int main() {
    TestPush<list<int> >("list_push_test.txt");
    TestPush<vector<int> >("vector_push_test.txt");
    TestPush<deque<int> >("deque_push_test.txt");

    TestAccess<vector<int> >("vector_access_test.txt");
    TestAccess<deque<int> >("deque_access_test.txt");
    return 0;
}

