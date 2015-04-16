#include "StrReplaceModifier.h"
#include <sstream>

using namespace std;

void StrReplaceModifier::Init(const string& src, const string& dst) {
    m_src = src;
    m_dst = dst;
}

bool StrReplaceModifier::ModifyMsg(const string& inMsg, string& outMsg) {
    ostringstream oss;

    const char* inMsgStr = inMsg.c_str();
    size_t pos = 0;
    while(true) {
        size_t newPos = inMsg.find(m_src, pos);
        if (newPos != string::npos) {
            oss.write(inMsgStr + pos, newPos - pos);
            oss << m_dst;
            pos = newPos + m_src.size();
        } else {
            oss.write(inMsgStr + pos, inMsg.size() - pos);
            break;
        }
    }
    outMsg = oss.str();
    return true;
}

