#ifndef GB_TO_UTF8_MODIFIER_H
#define GB_TO_UTF8_MODIFIER_H

#include "logger.h"

class GbToUtf8Modifier : public MsgModifier {
public:
    bool Init();
    bool ModifyMsg(const string& inMsg, string& outMsg);
};

#endif

