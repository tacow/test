#include "logger.h"
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TZ_ADJUST (8 * 3600)    // 将UTC时间转换为东8区北京时间

void DeleteMsgBuf(void* ptr) {
    delete[] (char*)ptr;
}

MsgLogger::MsgLogger() {
    m_logFile = NULL;
    m_maxFileLen = 104857600;
    m_fileLen = 0;

    m_sep = "|";

    memset(m_levels, 0, sizeof(m_levels));
    m_levels[LOG_ERROR] = "Error";
    m_levels[LOG_WARNING] = "Warning";
    m_levels[LOG_NOTICE] = "Notice";
    m_levels[LOG_INFO] = "Info";
    m_levels[LOG_DEBUG] = "Debug";
    m_levels[LOG_VERBOSE] = "Verbose";

    m_maxLevel = LOG_INFO;
    m_printToScreen = false;
    m_hasMsgModifier = false;

    m_ttLogDay = 0;

    m_failed = false;
    m_lastFailTime = 0.0;

    pthread_mutex_init(&m_mutex, NULL);

    pthread_key_create(&m_msgBufKey, DeleteMsgBuf);
}

MsgLogger::~MsgLogger() {
    pthread_key_delete(m_msgBufKey);

    Close();

    pthread_mutex_destroy(&m_mutex);

    pthread_key_delete(m_msgBufKey);
}

bool MsgLogger::Init(const char* prefix, const char* logPath) {
    m_prefix = prefix;
    m_logPath = logPath;
    m_logFileName = m_logPath + "/" + m_prefix + ".log";
    m_historyFileNamePattern = m_logPath + "/" + m_prefix + "_%Y%m%d%H%M%S.log";

    time_t ttNow = time(NULL);
    time_t ttNowAdj = ttNow + TZ_ADJUST;
    m_ttLogDay = ttNowAdj / (24 * 3600);

    tm tmNow;
    localtime_r(&ttNow, &tmNow);
    char strLogDay[256];
    strftime(strLogDay, 256, "%Y-%m-%d", &tmNow);
    m_strLogDay = strLogDay;

    return DoDivide();
}

void MsgLogger::Close() {
    if (m_logFile) {
        fclose(m_logFile);
        m_logFile = NULL;
        m_fileLen = 0;
    }
}

void MsgLogger::SetMaxFileLen(int maxFileLen) {
    m_maxFileLen = maxFileLen;
}

int MsgLogger::GetMaxFileLen() {
    return m_maxFileLen;
}

void MsgLogger::SetSeperator(const char* sep) {
    m_sep = sep;
}

const char* MsgLogger::GetSeperator() {
    return m_sep.c_str();
}

void MsgLogger::SetMaxLevel(int level) {
    m_maxLevel = level;
}

int MsgLogger::GetMaxLevel() {
    return m_maxLevel;
}

void MsgLogger::SetPrintToScreen(bool printToScreen) {
    m_printToScreen = printToScreen;
}

bool MsgLogger::GetPrintToScreen() {
    return m_printToScreen;
}

void MsgLogger::AddMsgModifier(MsgModifier* msgFilter) {
    m_hasMsgModifier = true;
    m_msgModifiers.push_back(msgFilter);
}

bool MsgLogger::VLog(int level, const char* format, ...) {
    if (level > m_maxLevel)
        return true;

    char* msgBuf = (char*)pthread_getspecific(m_msgBufKey);
    if (!msgBuf) {
        msgBuf = new char[MSG_BUF_SIZE];
        pthread_setspecific(m_msgBufKey, msgBuf);
    }

    va_list ap;
    va_start(ap, format);
    vsnprintf(msgBuf, MSG_BUF_SIZE, format, ap);
    va_end(ap);

    return Log(msgBuf, level);
}

bool MsgLogger::Log(const char* msg, int level) {
    if (level > m_maxLevel)
        return true;

    size_t msgLen = 0;
    string oldMsg;
    if (m_hasMsgModifier) {
        oldMsg = msg;
        string newMsg;
        for(list<MsgModifier*>::iterator it = m_msgModifiers.begin(); it != m_msgModifiers.end(); ++it) {
            MsgModifier* msgModifier = *it;
            if (!msgModifier->ModifyMsg(oldMsg, newMsg))
                return false;
            oldMsg.swap(newMsg);
            newMsg.clear();
        }
        msg = oldMsg.c_str();
        msgLen = oldMsg.size();
    } else {
        msgLen = strlen(msg);
    }

    pthread_mutex_lock(&m_mutex);

    timeval tvNow;
    gettimeofday(&tvNow, NULL);
    time_t ttNow = tvNow.tv_sec;
    time_t ttNowAdj = ttNow + TZ_ADJUST;
    time_t day = ttNowAdj / (24 * 3600);
    int timeOfDay = ttNowAdj % (24 * 3600);
    int hour = timeOfDay / 3600;
    int timeOfHour = timeOfDay % 3600;
    int min = timeOfHour / 60;
    int sec = timeOfHour % 60;

    bool dayChanged = (day != m_ttLogDay);
    if (dayChanged) {
        m_ttLogDay = day;

        tm tmNow;
        localtime_r(&ttNow, &tmNow);
        char strLogDay[256];
        strftime(strLogDay, 256, "%Y-%m-%d", &tmNow);
        m_strLogDay = strLogDay;
    }

    int tid = syscall(SYS_gettid);

    char title[4096];
    size_t titleLen = snprintf(title, 4096, "%s %02d:%02d:%02d.%06d%s%s%s%d%s", m_strLogDay.c_str(), hour, min, sec, (int)tvNow.tv_usec, m_sep.c_str(), m_levels[level], m_sep.c_str(), tid, m_sep.c_str());

    if (m_printToScreen) {
        if (level >= LOG_INFO)
            printf("%s%s\n", title, msg);
        else
            fprintf(stderr, "%s%s\n", title, msg);
    }

    if (m_failed) {
        if (GetMonotonicTime() >= m_lastFailTime + 1.0) {
            m_failed = false;
            m_lastFailTime = 0.0;
        }
    }

    bool succ;
    if (m_failed) {
        succ = false;
    } else {
        succ = true;
        if (m_logFile == NULL) {
            succ = DoDivide();
        } else if (m_fileLen > m_maxFileLen || dayChanged) {
            Close();
            succ = DoDivide();
        }

        if (succ) {
            size_t logLineLen = titleLen + msgLen + 1;
            succ = ((int)logLineLen == fprintf(m_logFile, "%s%s\n", title, msg));
            m_fileLen += logLineLen;
        }

        if (succ)
            succ = (0 == fflush(m_logFile));

        if (!succ) {
            Close();

            m_failed = true;
            m_lastFailTime = GetMonotonicTime();
        }
    }

    pthread_mutex_unlock(&m_mutex);

    return succ;
}

double MsgLogger::GetMonotonicTime() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

bool MsgLogger::DoDivide() {
    bool logFileExist = false;
    struct stat st;
    if (0 == stat(m_logFileName.c_str(), &st))
        logFileExist = (st.st_size > 0);

    if (logFileExist) {
        time_t ttNow = time(NULL);
        tm tmNow;
        localtime_r(&ttNow, &tmNow);

        char historyFileName[256];
        strftime(historyFileName, 256, m_historyFileNamePattern.c_str(), &tmNow);

        rename(m_logFileName.c_str(), historyFileName);
    }

    m_logFile = fopen(m_logFileName.c_str(), "wb");
    m_fileLen = 0;
    return (m_logFile != NULL);
}

