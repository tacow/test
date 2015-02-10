#include "ref_count.h"
#include <stdio.h>

class MyObject : public RefCountObject<MyObject> {
public:
    MyObject() { printf("Ctor\n"); }
    ~MyObject() { printf("Dtor\n"); }
};

int main() {
    MyObject* myObj = new MyObject();

    myObj->AddRef();
    printf("%d\n", myObj->GetRefCount());

    myObj->Release();
    printf("%d\n", myObj->GetRefCount());

    myObj->Release();
    return 0;
}

