#include <stdio.h>
#include <set>

using namespace std;

int main() {
    set<int> si;
    si.insert(2);
    si.insert(3);
    si.insert(6);
    si.insert(7);
    si.insert(22);
    si.insert(16);
    si.insert(9);

    set<int>::iterator it = si.begin();
    while(it != si.end()) {
        int n = *it;
        if (n % 2 == 1) {
            set<int>::iterator nit = it;
            ++nit;
            si.erase(it);
            it = nit;
        } else {
            ++it;
        }
    }

    for(set<int>::iterator it = si.begin(); it != si.end(); ++it) {
        int n = *it;
        printf("%d\n", n);
    }
    return 0;
}

