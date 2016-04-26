#ifndef FT_LOGGER_H
#define FT_LOGGER_H

#include "MsgLogger.h"
#include "CharsetConvert.h"

#define LOG_ERROR    0
#define LOG_WARNING  1
#define LOG_NOTICE   2
#define LOG_INFO     3
#define LOG_DEBUG    4
#define LOG_VERBOSE  5

#define MSG_BUF_SIZE           1024
#define MSG_BUF_CHUNK_SIZE     (1024 * 1024)
#define MSG_STRUCT_CHUNK_SIZE  (1024 * 1024)

class DateStrCache {
public:
    DateStrCache();
    ~DateStrCache();

    const char* GetDateStr(time_t ttTime);

private:
    time_t m_ttCachedDate;
    string m_strCachedDate;
};

class FTLogger;

class FTMsg : public IMsg {
public:
    size_t ToStr(char* buf, size_t bufSize);
    void Release();

public:
    char*  m_msgBuf;
    size_t m_msgLen;
    bool   m_bufFromPool;

    timeval m_msgTime;
    int     m_level;
    int     m_threadID;

    FTLogger* m_logger;
};

class FTLogger {
public:
    FTLogger();
    ~FTLogger();

    bool Init(const char* prefix, const char* logPath);
    void Close(bool immediate = false);

    // 设置和获取最大日志级别，默认为INFO级别
    int  GetMaxLevel();
    void SetMaxLevel(int level);

    void Log(int level, const char* format, ...);
    
    const char* GetDateStr(time_t ttTime);
    void RleaseMsg(FTMsg* msg);

private:
    int m_maxLevel;

    DateStrCache m_dateStrCache;
    MsgLogger m_msgLogger;

    MemPool m_msgBufPool;
    MemPool m_msgStructPool;
    pthread_mutex_t m_poolMutex;
};

#define VLog(logger, level, format, ...) \
    logger.Log(level, format ",%s:%d", ##__VA_ARGS__, __FILE__, __LINE__)

#endif

