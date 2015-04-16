#include "GbToUtf8Modifier.h"

bool GbToUtf8Modifier::Init() {
    return true;
}

bool GbToUtf8Modifier::ModifyMsg(const string& inMsg, string& outMsg) {
    outMsg = inMsg;
    return true;
}

