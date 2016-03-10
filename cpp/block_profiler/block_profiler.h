#ifndef BLOCK_PROFILER_H
#define BLOCK_PROFILER_H

#ifdef ENABLE_BLOCK_PROFILE

#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/time.h>

class BlockProfiler {
public:
    BlockProfiler(const char* blockName) {
        m_blockName = blockName;
        int tid = syscall(SYS_gettid);
        fprintf(g_logFile, "#%d >>> %s\n", tid, blockName);
        gettimeofday(&m_startTime, NULL);
    }

    ~BlockProfiler() {
        timeval endTime;
        gettimeofday(&endTime, NULL);
        long consumedTime = (endTime.tv_sec - m_startTime.tv_sec) * 1000000 + (endTime.tv_usec - m_startTime.tv_usec);
        int tid = syscall(SYS_gettid);
        fprintf(g_logFile, "#%d <<< %s %ld\n", tid, m_blockName, consumedTime);
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
    const char* m_blockName;
    timeval m_startTime;

    static FILE* g_logFile;
};

#define BLOCK_PROFILER_INIT(logFile) BlockProfiler::Init(logFile);

#define BLOCK_PROFILER_CLOSE() BlockProfiler::Close();

#define BLOCK_PROFILE_ENTRY(name) { BlockProfiler blockProfiler(name);

#define BLOCK_PROFILE_EXIT() }

#else

#define BLOCK_PROFILER_INIT(logfile)

#define BLOCK_PROFILER_CLOSE()

#define BLOCK_PROFILE_ENTRY(name)

#define BLOCK_PROFILE_EXIT()

#endif

#endif

