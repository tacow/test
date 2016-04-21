#ifndef MSG_LOGGER_H
#define MSG_LOGGER_H

#include <string>
#include "MsgQueue.h"

using namespace std;

#define MAX_MSG_LEN  (1024 * 1024)

class IMsg {
public:
    // 将消息结构体转换为字符串，字符串末尾必须有"\n"，但不必有"\0"
    // buf为提供的字符串缓冲区，bufSize为缓冲区最大长度，日志超长需截断
    // 返回值为字符串长度
    virtual size_t ToStr(char* buf, size_t bufSize) = 0;

    // 释放消息结构体
    virtual void Release() = 0;
};

class MsgLogger {
public:
    MsgLogger();
    ~MsgLogger();

    // 初始化日志
    // prefix为日志文件名前缀：
    //     * 当前正在写的日志文件将被命名为[prefix].log
    //     * 历史日志文件将被命名为[prefix]_[yyyymmddHHMMSS].log，文件名中的日期时间代表日志被切分时的时间
    // logPath为日志存放的路径，默认为当前目录
    // maxFileLen为日志文件最大长度，超过该长度则自动切分，默认为100M
    void Init(const char* prefix, const char* logPath = ".", int maxFileLen = 104857600); 

    // 关闭日志
    void Close();

    // 设置和获取在屏幕上输出日志类型
    // 0代表不输出到屏幕，1代表输出到stdout，2代表输出到stderr，默认为不输出
    int GetPrintToScreen();
    void SetPrintToScreen(int printToScreen = 1);

    // 输出日志
    void Log(IMsg* msg);

private:
    static void* WriterThreadS(void* ptr);
    void WriterThread();

    double GetMonotonicTime();
    void CloseLogFile();
    bool DivideLogFile();

    string m_prefix;
    string m_logPath;
    string m_logFileName;
    string m_historyFileNamePattern;
    FILE*  m_logFile;

    long   m_maxFileLen;
    long   m_fileLen;

    int    m_printToScreen;
    
    char*  m_msgBuf;

    bool   m_failed;
    double m_lastFailTime;

    pthread_t m_writerThread;
    MsgQueue m_msgQueue;
    volatile bool m_stoped;
};

#endif

