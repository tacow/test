#include "GbToUtf8Modifier.h"

#define INVALID_CONVERTER ((CharsetConvert*)-1)

void DeleteConverter(void* ptr) {
    CharsetConvert* converter = (CharsetConvert*)ptr;
    if (ptr != INVALID_CONVERTER)
        delete (CharsetConvert*)converter;
}

GbToUtf8Modifier::GbToUtf8Modifier() {
    pthread_key_create(&m_key, DeleteConverter);
}

GbToUtf8Modifier::~GbToUtf8Modifier() {
    pthread_key_delete(m_key);
}

bool GbToUtf8Modifier::Init() {
    return (NULL != GetThreadConverter());
}

bool GbToUtf8Modifier::ModifyMsg(const string& inMsg, string& outMsg) {
    CharsetConvert* converter = GetThreadConverter();
    if (!converter)
        return false;
    return converter->Convert(inMsg, outMsg);
}

CharsetConvert* GbToUtf8Modifier::GetThreadConverter() {
    CharsetConvert* converter = (CharsetConvert*)pthread_getspecific(m_key);
    if (!converter) {
        converter = new CharsetConvert();
        if (!converter->Init("GB18030", "UTF-8")) {
            delete converter;
            converter = INVALID_CONVERTER;
        }
        pthread_setspecific(m_key, converter);
    }

    if (converter == INVALID_CONVERTER)
        return NULL;
    else
        return converter;
}

