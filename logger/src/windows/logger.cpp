#include "logger.h"
#include <sys/timeb.h>
#include <sys/types.h>
#include <sys/stat.h>

#define TZ_ADJUST (8 * 3600)    // 将UTC时间转换为东8区北京时间

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

    m_mutex = CreateMutex( NULL, FALSE, NULL );
}

MsgLogger::~MsgLogger() {
    Close();

    CloseHandle(m_mutex);
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
    localtime_s(&tmNow, &ttNow);
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

    char* msgBuf = new char[MSG_BUF_SIZE];

    va_list ap;
    va_start(ap, format);
    vsnprintf_s(msgBuf, MSG_BUF_SIZE, _TRUNCATE, format, ap);
    va_end(ap);

    bool succ = Log(msgBuf, level);

    delete[] msgBuf;
    return succ;
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

    WaitForSingleObject(m_mutex, INFINITE);

    _timeb tbNow;
    _ftime_s(&tbNow);
    time_t ttNow = tbNow.time;
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
        localtime_s(&tmNow, &ttNow);
        char strLogDay[256];
        strftime(strLogDay, 256, "%Y-%m-%d", &tmNow);
        m_strLogDay = strLogDay;
    }

    int tid = GetCurrentThreadId();

    char title[256];
    size_t titleLen = _snprintf_s(title, 256, _TRUNCATE, "%s %02d:%02d:%02d.%03d%s%s%s%d%s", m_strLogDay.c_str(), hour, min, sec, (int)tbNow.millitm, m_sep.c_str(), m_levels[level], m_sep.c_str(), tid, m_sep.c_str());

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
            size_t logLineLen = titleLen + msgLen + 2;
            succ = ((int)logLineLen == fprintf(m_logFile, "%s%s\r\n", title, msg));
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

    ReleaseMutex(m_mutex);

    return succ;
}

double MsgLogger::GetMonotonicTime() {
    return GetTickCount64() / 1000.0;
}

bool MsgLogger::DoDivide() {
    bool logFileExist = false;
    struct _stat64 st;
    if (0 == _stat64(m_logFileName.c_str(), &st))
        logFileExist = (st.st_size > 0);

    if (logFileExist) {
        time_t ttNow = time(NULL);
        tm tmNow;
        localtime_s(&tmNow, &ttNow);

        char historyFileName[256];
        strftime(historyFileName, 256, m_historyFileNamePattern.c_str(), &tmNow);

        rename(m_logFileName.c_str(), historyFileName);
    }

    fopen_s(&m_logFile, m_logFileName.c_str(), "wb");
    m_fileLen = 0;
    return (m_logFile != NULL);
}

