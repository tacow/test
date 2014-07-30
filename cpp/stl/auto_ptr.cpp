#include <stdio.h>
#include <memory>

using namespace std;

class A {
public:
    A(int n) {
        m_n = n;
        printf("Ctor\n");
    }

    ~A() {
        printf("Dtor\n");
    }

    int m_n;
};

int main() {
    auto_ptr<A> pa;
    pa = auto_ptr<A>(new A(5));
    printf("%d\n", pa->m_n);
    pa = auto_ptr<A>(new A(6));
    printf("%d\n", pa->m_n);
    return 0;
}

