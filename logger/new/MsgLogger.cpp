#include <stdio.h>
#include "MsgLogger.h"

MsgLogger::MsgLogger() {
    m_logFile = NULL;
    m_maxFileLen = 104857600;
    m_fileLen = 0;

    m_printToScreen = 0;

    m_msgBuf = new char[MAX_MSG_LEN];
    m_convertor = NULL;

    m_failed = false;
    m_lastFailTime = 0.0;

    m_stoped = true;
}

MsgLogger::~MsgLogger() {
    Close(true);

    ClearCharsetConvert();
    delete[] m_msgBuf;
}

void MsgLogger::Init(const char* prefix, const char* logPath, int maxFileLen) {
    m_prefix = prefix;
    m_logPath = logPath;
    m_logFileName = m_logPath + "/" + m_prefix + ".log";
    m_historyFileNamePattern = m_logPath + "/" + m_prefix + "_%Y%m%d%H%M%S.log";

    m_maxFileLen = maxFileLen;

    m_stoped = false;
    pthread_create(&m_writerThread, NULL, MsgLogger::WriterThreadS, this);
}

void MsgLogger::Close(bool immediate) {
    if (m_stoped)
        return;

    if (immediate)
        m_stoped = true;
    m_msgQueue.Push(NULL);
    pthread_join(m_writerThread, NULL);
}

void MsgLogger::SetPrintToScreen(int printToScreen) {
    m_printToScreen = printToScreen;
}

int MsgLogger::GetPrintToScreen() {
    return m_printToScreen;
}

void MsgLogger::ClearCharsetConvert() {
    if (m_convertor) {
        delete m_convertor;
        m_convertor = NULL;
    }
}

bool MsgLogger::SetCharsetConvert(const char* toCharset, const char* fromCharset) {
    ClearCharsetConvert();

    if (!toCharset || !fromCharset)
        return true;

    m_convertor = new CharsetConvert();
    if (!m_convertor->Init(toCharset, fromCharset, MAX_MSG_LEN)) {
        ClearCharsetConvert();
        return false;
    }
    return true;
}

void MsgLogger::Log(IMsg* msg) {
    m_msgQueue.Push(msg);
}

void* MsgLogger::WriterThreadS(void* ptr) {
    MsgLogger* logger = (MsgLogger*)ptr;
    logger->WriterThread();
    return NULL;
}

void MsgLogger::WriterThread() {
    DivideLogFile();

    while(!m_stoped) {
        IMsg* msg = (IMsg*)m_msgQueue.Pop();
        if (msg == NULL)
            break;

        size_t len = msg->ToStr(m_msgBuf, MAX_MSG_LEN);
        msg->Release();

        char*  msgBuf;
        size_t msgLen;
        if (!m_convertor) {
            msgBuf = m_msgBuf;
            msgLen = len;
        } else {
            msgBuf = m_convertor->Convert(m_msgBuf, len, &msgLen);
            if (!msgBuf)
                continue;
        }

        if (m_printToScreen == 1)
            fwrite(msgBuf, 1, msgLen, stdout);

        if (m_printToScreen == 2)
            fwrite(msgBuf, 1, msgLen, stderr);

        if (m_failed) {
            if (GetMonotonicTime() >= m_lastFailTime + 1.0) {
                m_failed = false;
                m_lastFailTime = 0.0;
            } else {
                continue;
            }
        }

        bool succ = true;
        if (m_logFile == NULL) {
            succ = DivideLogFile();
        } else if (m_fileLen > m_maxFileLen) {
            CloseLogFile();
            succ = DivideLogFile();
        }

        if (succ) {
            succ = (len == fwrite(msgBuf, 1, msgLen, m_logFile));
            m_fileLen += len;
        }

        if (succ)
            succ = (0 == fflush(m_logFile));

        if (!succ) {
            CloseLogFile();

            m_failed = true;
            m_lastFailTime = GetMonotonicTime();
        }
    }

    CloseLogFile();

    void* ptr;
    void* iter = NULL;
    while(m_msgQueue.GetNext(ptr, iter)) {
        if (ptr == NULL)
            continue;

        IMsg* msg = (IMsg*)ptr;
        msg->Release();
    }
    m_stoped = true;
}

double MsgLogger::GetMonotonicTime() {
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1000000000.0;
}

bool MsgLogger::DivideLogFile() {
    time_t ttNow = time(NULL);
    tm tmNow;
    localtime_r(&ttNow, &tmNow);

    char historyFileName[256];
    strftime(historyFileName, 256, m_historyFileNamePattern.c_str(), &tmNow);

    rename(m_logFileName.c_str(), historyFileName);

    m_logFile = fopen(m_logFileName.c_str(), "wb");
    m_fileLen = 0;
    return (m_logFile != NULL);
}

void MsgLogger::CloseLogFile() {
    if (m_logFile) {
        fclose(m_logFile);
        m_logFile = NULL;
        m_fileLen = 0;
    }
}

