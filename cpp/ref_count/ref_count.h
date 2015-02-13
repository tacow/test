#ifndef REF_COUNT_H
#define REF_COUNT_H

template<class DerivedClass>
class RefCountObject {
public:
    RefCountObject(int refCount = 1) : m_refCount(refCount) {}
    virtual ~RefCountObject() {}

    void AddRef() { __sync_add_and_fetch(&m_refCount, 1); }
    void Release() { if (__sync_sub_and_fetch(&m_refCount, 1) == 0) delete (DerivedClass*)this; }

    int GetRefCount() { return m_refCount; }

private:
    int m_refCount;
};

#endif

