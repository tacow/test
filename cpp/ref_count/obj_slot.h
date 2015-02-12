#ifndef OBJ_SLOT_H
#define OBJ_SLOT_H

#include <pthread.h>

template<class Object>
class ObjectSlot {
public:
    ObjectSlot() : m_obj(NULL) {
        pthread_mutex_init(&m_lock, NULL);
    }

    virtual ~ObjectSlot() {
        if (m_obj)
            m_obj->Release();

        pthread_mutex_destroy(&m_lock);
    }

    void Set(Object* obj) {
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

