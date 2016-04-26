#include "FTLogger.h"
#include <stdio.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/syscall.h>

#define TZ_ADJUST (8 * 3600)    // 将UTC时间转换为东8区北京时间

DateStrCache::DateStrCache() {
    m_ttCachedDate = 0;
}

DateStrCache::~DateStrCache() {
}

const char* DateStrCache::GetDateStr(time_t ttTime) {
    time_t ttTimeAdj = ttTime + TZ_ADJUST;
    time_t ttDate = ttTimeAdj / (24 * 3600);
    if (ttDate != m_ttCachedDate) {
        tm tmTime;
        localtime_r(&ttTime, &tmTime);
        char strDate[256];
        strftime(strDate, 256, "%Y-%m-%d", &tmTime);

        m_ttCachedDate = ttDate;
        m_strCachedDate = strDate;
    }
    return m_strCachedDate.c_str();
}

size_t FTMsg::ToStr(char* buf, size_t bufSize) {
    static const char* levelStr[] = {
        "Error",
        "Warning",
        "Notice",
        "Info",
        "Debug",
        "Verbose",
    };

    timeval tvNow;
    gettimeofday(&tvNow, NULL);
    time_t ttNow = tvNow.tv_sec;
    time_t ttNowAdj = ttNow + TZ_ADJUST;
    int timeOfDay = ttNowAdj % (24 * 3600);
    int hour = timeOfDay / 3600;
    int timeOfHour = timeOfDay % 3600;
    int min = timeOfHour / 60;
    int sec = timeOfHour % 60;
    size_t len = snprintf(buf, bufSize, "%s-%d-%d-%d.%06d,%s,%d,%s", m_logger->GetDateStr(ttNow), hour, min, sec, (int)tvNow.tv_usec, levelStr[m_level], m_threadID, m_msgBuf);
    if (len > bufSize - 1)
        len = bufSize - 1;
    buf[len] = '\n';
    return len + 1;
}

void FTMsg::Release() {
    m_logger->RleaseMsg(this);
}

FTLogger::FTLogger() : m_msgBufPool(MSG_BUF_SIZE, MSG_BUF_CHUNK_SIZE), m_msgStructPool(sizeof(FTMsg), MSG_STRUCT_CHUNK_SIZE, DefCtor<FTMsg>, DefDtor<FTMsg>) {
    m_maxLevel = LOG_INFO;

    pthread_mutex_init(&m_poolMutex, NULL);
}

FTLogger::~FTLogger() {
    pthread_mutex_destroy(&m_poolMutex);
}

bool FTLogger::Init(const char* prefix, const char* logPath) {
    if (!m_msgLogger.SetCharsetConvert("UTF-8", "GB18030"))
        return false;
    m_msgLogger.Init(prefix, logPath);
    return true;
}

void FTLogger::Close(bool immediate) {
    m_msgLogger.Close(immediate);
}

void FTLogger::SetMaxLevel(int level) {
    m_maxLevel = level;
}

int FTLogger::GetMaxLevel() {
    return m_maxLevel;
}

void FTLogger::Log(int level, const char* format, ...) {
    if (level > m_maxLevel)
        return;

    pthread_mutex_lock(&m_poolMutex);
    char* msgBuf = (char*)m_msgBufPool.Alloc();
    FTMsg* msg = (FTMsg*)m_msgStructPool.Alloc();
    pthread_mutex_unlock(&m_poolMutex);

    va_list ap;
    va_start(ap, format);
    int len = vsnprintf(msgBuf, MSG_BUF_SIZE, format, ap);
    va_end(ap);
    if (len <= MSG_BUF_SIZE - 1) {
        msg->m_msgBuf = msgBuf;
        msg->m_msgLen = len;
        msg->m_bufFromPool = true;
    } else {
        pthread_mutex_lock(&m_poolMutex);
        m_msgBufPool.Free(msgBuf);
        pthread_mutex_unlock(&m_poolMutex);

        msg->m_msgBuf = new char[len + 1];

        va_list ap;
        va_start(ap, format);
        vsnprintf(msg->m_msgBuf, len + 1, format, ap);
        va_end(ap);

        msg->m_msgLen = len;
        msg->m_bufFromPool = false;
    }

    gettimeofday(&msg->m_msgTime, NULL);
    msg->m_level = level;
    msg->m_threadID = syscall(SYS_gettid);
    msg->m_logger = this;
    m_msgLogger.Log(msg);
}

const char* FTLogger::GetDateStr(time_t ttTime) {
    return m_dateStrCache.GetDateStr(ttTime);
}

void FTLogger::RleaseMsg(FTMsg* msg) {
    pthread_mutex_lock(&m_poolMutex);
    if (msg->m_bufFromPool) {
        m_msgBufPool.Free(msg->m_msgBuf);
        m_msgStructPool.Free(msg);
    } else {
        delete[] msg->m_msgBuf;
        m_msgStructPool.Free(msg);
    }
    pthread_mutex_unlock(&m_poolMutex);
}

