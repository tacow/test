#include "copyfile.h"

bool CopyFileByReadWrite(const string& srcPath, const string& dstPath, off_t offset, size_t len, string& errMsg, mode_t mode)
{
    int srcFile = open(srcPath.c_str(), O_RDONLY);
    if (srcFile == -1)
    {
        char msgBuf[1024];
        snprintf(msgBuf, sizeof(msgBuf), "Can't open source path: %s", srcPath.c_str());
        errMsg = msgBuf;
        return false;
    }

    int dstFile = open(dstPath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, mode);
    if (dstFile == -1)
    {
        close(srcFile);

        char msgBuf[1024];
        snprintf(msgBuf, sizeof(msgBuf), "Can't open destination path: %s", srcPath.c_str());
        errMsg = msgBuf;
        return false;
    }

    off_t lres = lseek(srcFile, offset, SEEK_SET);
    if (lres == (off_t)-1)
    {
        close(srcFile);
        close(dstFile);

        char msgBuf[1024];
        snprintf(msgBuf, sizeof(msgBuf), "Failed to execute lseek: errno=%d", errno);
        errMsg = msgBuf;
        return false;
    }
    if (lres != offset)
    {
        close(srcFile);
        close(dstFile);

        char msgBuf[1024];
        snprintf(msgBuf, sizeof(msgBuf), "lseek result is not same as expected: result=%ld, expected=%ld", (long)lres, (long)offset);
        errMsg = msgBuf;
        return false;
    }

    const size_t DATA_BUF_LEN = 1024 * 1024;
    char* dataBuf = (char*)malloc(DATA_BUF_LEN);
    while (len)
    {
        size_t readLen = std::min(len, DATA_BUF_LEN);
        ssize_t res = read(srcFile, dataBuf, readLen);
        if (res == -1)
        {
            if (errno == EINTR)
                continue;

            close(srcFile);
            close(dstFile);
            free(dataBuf);

            char msgBuf[1024];
            snprintf(msgBuf, sizeof(msgBuf), "Failed to execute read: errno=%d", errno);
            errMsg = msgBuf;
            return false;
        }
        if (res <= 0)
        {
            close(srcFile);
            close(dstFile);
            free(dataBuf);

            char msgBuf[1024];
            snprintf(msgBuf, sizeof(msgBuf), "Failed to execute read: result=%ld", (long)res);
            errMsg = msgBuf;
            return false;
        }
        readLen = (size_t)res;

        size_t writeLen = 0;
        while (writeLen < readLen)
        {
            res = write(dstFile, &(dataBuf[writeLen]), readLen - writeLen);
            if (res == -1)
            {
                if (errno == EINTR)
                    continue;

                close(srcFile);
                close(dstFile);
                free(dataBuf);

                char msgBuf[1024];
                snprintf(msgBuf, sizeof(msgBuf), "Failed to execute write: errno=%d", errno);
                errMsg = msgBuf;
                return false;
            }
            if (res <= 0)
            {
                close(srcFile);
                close(dstFile);
                free(dataBuf);

                char msgBuf[1024];
                snprintf(msgBuf, sizeof(msgBuf), "Failed to execute write: result=%ld", (long)res);
                errMsg = msgBuf;
                return false;
            }
            writeLen += (size_t)res;
        }
        len -= writeLen;
    }

    close(srcFile);
    close(dstFile);
    free(dataBuf);
    return true;
}

bool CopyFile(const string& srcPath, const string& dstPath, off_t offset, size_t len, string& errMsg, mode_t mode, bool checkFileSize)
{
    if (checkFileSize)
    {
        struct stat fileStat;
        int res = stat(srcPath.c_str(), &fileStat);
        if (res == -1)
        {
            char msgBuf[1024];
            snprintf(msgBuf, sizeof(msgBuf), "Failed to execute stat: errno=%d", errno);
            errMsg = msgBuf;
            return false;
        }

        if (fileStat.st_size < offset + (off_t)len)
        {
            char msgBuf[1024];
            snprintf(msgBuf, sizeof(msgBuf), "The source file is smaller than expected, expectedSize=%ld, actualSize=%ld", (long)offset + (long)len, (long)fileStat.st_size);
            errMsg = msgBuf;
            return false;
        }
    }

    bool succ = CopyFileByReadWrite(srcPath, dstPath, offset, len, errMsg, mode);
    return succ;
}

bool CopyFile(const string& srcPath, const string& dstPath, string& errMsg, mode_t mode)
{
    struct stat fileStat;
    int res = stat(srcPath.c_str(), &fileStat);
    if (res == -1)
    {
        char msgBuf[1024];
        snprintf(msgBuf, sizeof(msgBuf), "Failed to execute stat: errno=%d", errno);
        errMsg = msgBuf;
        return false;
    }

    return CopyFile(srcPath, dstPath, 0, fileStat.st_size, errMsg, mode, false);
}

