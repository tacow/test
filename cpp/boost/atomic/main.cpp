#include <stdio.h>
#include <boost/atomic.hpp>

using namespace boost;

int main() {
    atomic<unsigned long long> ai(5);
    printf("%llu\n", ai.load());
    ai.store(6);
    printf("%llu\n", ai.load());
    printf("%llu\n", ai.fetch_add(2));
    printf("%llu\n", ai.load());
    return 0;
}

