#ifndef FUNCTION_PROFILER_H
#define FUNCTION_PROFILER_H

#ifdef ENABLE_FUNCTION_PROFILE

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

class FunctionProfiler {
public:
    FunctionProfiler(const char* functionName) {
        m_functionName = functionName;
        m_blockName = NULL;
        gettimeofday(&m_functionStartTime, NULL);
        int tid = syscall(SYS_gettid);
        fprintf(g_logFile, "%ld.%06ld #%d ==> %s\n", (long)m_functionStartTime.tv_sec, (long)m_functionStartTime.tv_usec, tid, functionName);
    }

    ~FunctionProfiler() {
        timeval functionEndTime;
        gettimeofday(&functionEndTime, NULL);
        long consumedTime = (functionEndTime.tv_sec - m_functionStartTime.tv_sec) * 1000000 + (functionEndTime.tv_usec - m_functionStartTime.tv_usec);
        int tid = syscall(SYS_gettid);
        fprintf(g_logFile, "%ld.%06ld #%d <== %s %ld\n", (long)functionEndTime.tv_sec, (long)functionEndTime.tv_usec, tid, m_functionName, consumedTime);
    }

    void EnterBlock(const char* blockName) {
        m_blockName = blockName;
        gettimeofday(&m_blockStartTime, NULL);
        int tid = syscall(SYS_gettid);
        fprintf(g_logFile, "%ld.%06ld #%d --> %s_%s\n", (long)m_blockStartTime.tv_sec, (long)m_blockStartTime.tv_usec, tid, m_functionName, blockName);
    }

    void ExitBlock(const char* blockName) {
        if (!m_blockName || strcmp(blockName, m_blockName) != 0)
            return;

        timeval blockEndTime;
        gettimeofday(&blockEndTime, NULL);
        long consumedTime = (blockEndTime.tv_sec - m_blockStartTime.tv_sec) * 1000000 + (blockEndTime.tv_usec - m_blockStartTime.tv_usec);
        int tid = syscall(SYS_gettid);
        fprintf(g_logFile, "%ld.%06ld #%d <-- %s_%s %ld\n", (long)blockEndTime.tv_sec, (long)blockEndTime.tv_usec, tid, m_functionName, blockName, consumedTime);

        m_blockName = NULL;
    }

    static bool Init(const char* logFile) {
        g_logFile = fopen(logFile, "w");
        return (g_logFile != NULL);
    }

    static void Close() {
        if (g_logFile)
            fclose(g_logFile);
    }

private:
    const char* m_functionName;
    timeval m_functionStartTime;

    const char* m_blockName;
    timeval m_blockStartTime;

    static FILE* g_logFile;
};

#define FUNCTION_PROFILER_INIT(logFile) FunctionProfiler::Init(logFile)
#define FUNCTION_PROFILER_CLOSE() FunctionProfiler::Close()

#define FUNCTION_PROFILE(functionName) FunctionProfiler functionProfiler(functionName)

#define BLOCK_PROFILE_ENTRY(blockName) functionProfiler.EnterBlock(blockName)
#define BLOCK_PROFILE_EXIT(blockName) functionProfiler.ExitBlock(blockName)

#else

#define FUNCTION_PROFILER_INIT(logFile)
#define FUNCTION_PROFILER_CLOSE()

#define FUNCTION_PROFILE(functionName)

#define BLOCK_PROFILE_ENTRY(blockName)
#define BLOCK_PROFILE_EXIT(blockName)

#endif

#endif

