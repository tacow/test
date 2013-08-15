#include <stdio.h>
#include <queue>

using namespace std;

int main(int argc, char* argv[]) {
    queue<int> q;
    for(int i = 0; i < 10; ++i)
        q.push(i);
    while(!q.empty()) {
        printf("%d\n", q.front());
        q.pop();
    }
    return 0;
}

