#include "function_profiler.h"

#ifdef ENABLE_FUNCTION_PROFILE

const char* FunctionProfiler::g_logFilePrefix;
pthread_key_t FunctionProfiler::g_logFileKey;

FunctionProfiler::FunctionProfiler(const char* functionName) {
    m_functionName = functionName;
    m_blockName = NULL;
    gettimeofday(&m_functionStartTime, NULL);
    fprintf(GetLogFile(), "%ld.%06ld ==> %s\n", (long)m_functionStartTime.tv_sec, (long)m_functionStartTime.tv_usec, functionName);
}

FunctionProfiler::~FunctionProfiler() {
    timeval functionEndTime;
    gettimeofday(&functionEndTime, NULL);
    long consumedTime = (functionEndTime.tv_sec - m_functionStartTime.tv_sec) * 1000000 + (functionEndTime.tv_usec - m_functionStartTime.tv_usec);
    fprintf(GetLogFile(), "%ld.%06ld <== %s %ld\n", (long)functionEndTime.tv_sec, (long)functionEndTime.tv_usec, m_functionName, consumedTime);
}

void FunctionProfiler::EnterBlock(const char* blockName) {
    m_blockName = blockName;
    gettimeofday(&m_blockStartTime, NULL);
    fprintf(GetLogFile(), "%ld.%06ld --> %s_%s\n", (long)m_blockStartTime.tv_sec, (long)m_blockStartTime.tv_usec, m_functionName, blockName);
}

void FunctionProfiler::ExitBlock(const char* blockName) {
    if (!m_blockName || strcmp(blockName, m_blockName) != 0) {
        m_blockName = NULL;
        return;
    }

    timeval blockEndTime;
    gettimeofday(&blockEndTime, NULL);
    long consumedTime = (blockEndTime.tv_sec - m_blockStartTime.tv_sec) * 1000000 + (blockEndTime.tv_usec - m_blockStartTime.tv_usec);
    fprintf(GetLogFile(), "%ld.%06ld <-- %s_%s %ld\n", (long)blockEndTime.tv_sec, (long)blockEndTime.tv_usec, m_functionName, blockName, consumedTime);

    m_blockName = NULL;
}

FILE* FunctionProfiler::GetLogFile() {
    FILE* f = (FILE*)pthread_getspecific(g_logFileKey);
    if (!f) {
        char logFileName[256];
        int pid = (int)getpid();
        int tid = syscall(SYS_gettid);
        snprintf(logFileName, 256, "%s.%d.%d", g_logFilePrefix, pid, tid);
        f = fopen(logFileName, "w");
        pthread_setspecific(g_logFileKey, f);
    }
    return f;
}

void FunctionProfiler::Init(const char* logFilePrefix) {
    g_logFilePrefix = logFilePrefix;
    pthread_key_create(&g_logFileKey, NULL);
}

#endif

