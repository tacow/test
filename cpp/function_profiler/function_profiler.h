#ifndef FUNCTION_PROFILER_H
#define FUNCTION_PROFILER_H

#ifdef ENABLE_FUNCTION_PROFILE

#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>

class FunctionProfiler {
public:
    FunctionProfiler(const char* functionName);
    ~FunctionProfiler();

    void EnterBlock(const char* blockName);
    void ExitBlock(const char* blockName);

    static void Init(const char* logFilePrefix);

private:
    FILE* GetLogFile();

private:
    const char* m_functionName;
    timeval m_functionStartTime;

    const char* m_blockName;
    timeval m_blockStartTime;

    static const char* g_logFilePrefix;
    static pthread_key_t g_logFileKey;
};

#define FUNCTION_PROFILER_INIT(logFile) FunctionProfiler::Init(logFile)

#define FUNCTION_PROFILE(functionName) FunctionProfiler functionProfiler(functionName)

#define BLOCK_PROFILE_ENTRY(blockName) functionProfiler.EnterBlock(blockName)
#define BLOCK_PROFILE_EXIT(blockName) functionProfiler.ExitBlock(blockName)

#else

#define FUNCTION_PROFILER_INIT(logFile)

#define FUNCTION_PROFILE(functionName)

#define BLOCK_PROFILE_ENTRY(blockName)
#define BLOCK_PROFILE_EXIT(blockName)

#endif

#endif

