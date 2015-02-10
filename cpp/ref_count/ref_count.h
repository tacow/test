#ifndef REF_COUNT_H
#define REF_COUNT_H

template<class DerivedClass>
class RefCountObject {
public:
    RefCountObject(int refCount = 1) : m_refCount(refCount) {}
    virtual ~RefCountObject() {}

    void AddRef() { ++m_refCount; }
    void Release() { --m_refCount; if (m_refCount == 0) delete (DerivedClass*)this; }

    int GetRefCount() { return m_refCount; }

private:
    int m_refCount;
};

#endif

