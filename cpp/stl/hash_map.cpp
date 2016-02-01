#include <stdio.h>
#include <ext/hash_map>

using namespace __gnu_cxx;

int main() {
    hash_map<int, int> hmap;
    for(int i = 0; i < 1000; ++i)
        hmap[i] = i + 1;
    printf("%d %d\n", hmap[500], hmap[800]);
    return 0;
}

