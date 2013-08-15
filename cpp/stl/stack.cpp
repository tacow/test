#include <stdio.h>
#include <stack>

using namespace std;

int main(int argc, char* argv[]) {
    stack<int> stk;
    for(int i = 0; i < 10; ++i)
        stk.push(i);
    while(!stk.empty()) {
        printf("%d\n", stk.top());
        stk.pop();
    }
    return 0;
}

