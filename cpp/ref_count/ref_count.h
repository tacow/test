#ifndef REF_COUNT_H
#define REF_COUNT_H

#include <pthread.h>

template<class DerivedClass>
class RefCountObject {
public:
    RefCountObject(int refCount = 1) : m_refCount(refCount) {}
    virtual ~RefCountObject() {}

    void AddRef() {
        __sync_add_and_fetch(&m_refCount, 1);
    }

    void Release() {
        if (__sync_sub_and_fetch(&m_refCount, 1) == 0)
            delete (DerivedClass*)this;
    }

    int GetRefCount() {
        return m_refCount;
    }

private:
    int m_refCount;
};

template<class Object>
class RefCountObjectPtr {
public:
    RefCountObjectPtr() : m_obj(NULL) {
        pthread_mutex_init(&m_lock, NULL);
    }

    virtual ~RefCountObjectPtr() {
        if (m_obj)
            m_obj->Release();

        pthread_mutex_destroy(&m_lock);
    }

    void Set(Object* obj) {
        if (obj)
            obj->AddRef();

        pthread_mutex_lock(&m_lock);
        Object* oldObj = m_obj;
        m_obj = obj;
        pthread_mutex_unlock(&m_lock);

        if (oldObj)
            oldObj->Release();
    }

    Object* Get() {
        pthread_mutex_lock(&m_lock);
        Object* obj = m_obj;
        if (obj)
            obj->AddRef();
        pthread_mutex_unlock(&m_lock);

        return obj;
    }

private:
    Object* m_obj;
    pthread_mutex_t m_lock;
};

#endif

