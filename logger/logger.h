#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>
#include <string>
#include <list>

using namespace std;

#define LOG_ERROR    0
#define LOG_WARNING  1
#define LOG_NOTICE   2
#define LOG_INFO     3
#define LOG_DEBUG    4
#define LOG_VERBOSE  5

// 消息修改器接口
class MsgModifier {
public:
    // 消息修改函数，本函数需可以在多个线程中同时执行
    // inMsg代表修改前的消息字符串
    // outMsg代表修改后的消息字符串
    // 返回值代表修改是否成功，若不成功则该条日志将不会被输出
    virtual bool ModifyMsg(const string& inMsg, string& outMsg) = 0;
};

// 按日期和文件大小分割的消息日志类
class MsgLogger {
public:
    MsgLogger();
    virtual ~MsgLogger();

    // 初始化函数
    // prefix为日志文件名前缀：
    //     * 当前正在写的日志文件将被命名为[prefix].log
    //     * 历史日志文件将被命名为[prefix]_[yyyymmddHHMMSS].log，文件名中的日期时间代表日志被切分时的时间
    // logPath为日志存放的路径，默认为当前目录
    bool Init(const char* prefix, const char* logPath = "."); 

    // 关闭日志
    void Close();

    // 设置和获取最大日志文件长度，超过该长度将自动切分文件，默认为100M
    void SetMaxFileLen(int maxFileLen = 104857600);
    int  GetMaxFileLen();

    // 设置和获取字段分隔符，默认为"|"
    void SetSeperator(const char* sep = "|");
    const char* GetSeperator();

    // 设置和获取最大日志级别，默认为INFO级别
    void SetMaxLevel(int level = LOG_INFO);
    int  GetMaxLevel();

    // 设置和获取是否在屏幕上显示日志，默认为不显示
    void SetPrintToScreen(bool printToScreen = true);
    bool GetPrintToScreen();

    // 添加消息修改器，消息会按照修改器被添加的顺序被依次修改
    void AddMsgModifier(MsgModifier* msgFilter);

    // 记录日志
    // msg为日志消息
    // level为日志级别，默认为INFO级别
    bool Log(const char* msg, int level = LOG_INFO, const char* sourceFile = NULL, int lineNum = 0);

protected:
    double GetMonotonicTime();
    bool DoDivide();

    string m_prefix;
    string m_logPath;
    string m_logFileName;
    string m_historyFileNamePattern;
    FILE*  m_logFile;

    int    m_maxFileLen;
    int    m_fileLen;

    string m_sep;

    const char* m_levels[10];
    int    m_maxLevel;

    bool   m_printToScreen;
    
    bool   m_hasMsgModifier;
    list<MsgModifier*> m_msgModifiers;

    time_t m_ttLogDay;
    string m_strLogDay;

    bool   m_failed;
    double m_lastFailTime;

    pthread_mutex_t m_mutex;
};

#endif

