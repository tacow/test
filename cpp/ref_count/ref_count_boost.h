#ifndef REF_COUNT_H
#define REF_COUNT_H

#include <boost/atomic.hpp>

using namespace boost;

template<class DerivedClass>
class RefCountObject {
public:
    RefCountObject(int refCount = 1) : m_refCount(refCount) {}
    virtual ~RefCountObject() {}

    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) delete (DerivedClass*)this; }

    int GetRefCount() { return m_refCount; }

private:
    atomic_int m_refCount;
};

#endif

