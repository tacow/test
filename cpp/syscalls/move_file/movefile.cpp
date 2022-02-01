#include "movefile.h"

bool MoveFile(const string& srcPath, const string& dstPath, string& errMsg)
{
    int res = rename(srcPath.c_str(), dstPath.c_str());
    if (res == -1)
    {
        char msgBuf[1024];
        snprintf(msgBuf, sizeof(msgBuf), "Failed to execute rename: errno=%d", errno);
        errMsg = msgBuf;
        return false;
    }
    return true;
}
