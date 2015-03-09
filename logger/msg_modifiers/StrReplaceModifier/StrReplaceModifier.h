#ifndef STR_REPLACE_MODIFIER_H
#define STR_REPLACE_MODIFIER_H

#include "logger.h"

class StrReplaceModifier : public MsgModifier {
public:
    void Init(const string& src, const string& dst);
    bool ModifyMsg(const string& inMsg, string& outMsg);

private:
    string m_src;
    string m_dst;
};

#endif

