#ifndef GB_TO_UTF8_MODIFIER_H
#define GB_TO_UTF8_MODIFIER_H

#include "logger.h"
#include "CharsetConvert.h"
#include <deque>

using namespace std;

class GbToUtf8Modifier : public MsgModifier {
public:
    GbToUtf8Modifier();
    ~GbToUtf8Modifier();

    bool Init();
    bool ModifyMsg(const string& inMsg, string& outMsg);

private:
    CharsetConvert* GetThreadConverter();

    pthread_key_t m_key;
};

#endif

