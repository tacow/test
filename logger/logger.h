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

// ��Ϣ�޸����ӿ�
class MsgModifier {
public:
    // ��Ϣ�޸ĺ�����������������ڶ���߳���ͬʱִ��
    // inMsg�����޸�ǰ����Ϣ�ַ���
    // outMsg�����޸ĺ����Ϣ�ַ���
    // ����ֵ�����޸��Ƿ�ɹ��������ɹ��������־�����ᱻ���
    virtual bool ModifyMsg(const string& inMsg, string& outMsg) = 0;
};

// �����ں��ļ���С�ָ����Ϣ��־��
class MsgLogger {
public:
    MsgLogger();
    virtual ~MsgLogger();

    // ��ʼ������
    // prefixΪ��־�ļ���ǰ׺��
    //     * ��ǰ����д����־�ļ���������Ϊ[prefix].log
    //     * ��ʷ��־�ļ���������Ϊ[prefix]_[yyyymmddHHMMSS].log���ļ����е�����ʱ�������־���з�ʱ��ʱ��
    // logPathΪ��־��ŵ�·����Ĭ��Ϊ��ǰĿ¼
    bool Init(const char* prefix, const char* logPath = "."); 

    // �ر���־
    void Close();

    // ���úͻ�ȡ�����־�ļ����ȣ������ó��Ƚ��Զ��з��ļ���Ĭ��Ϊ100M
    void SetMaxFileLen(int maxFileLen = 104857600);
    int  GetMaxFileLen();

    // ���úͻ�ȡ�ֶηָ�����Ĭ��Ϊ"|"
    void SetSeperator(const char* sep = "|");
    const char* GetSeperator();

    // ���úͻ�ȡ�����־����Ĭ��ΪINFO����
    void SetMaxLevel(int level = LOG_INFO);
    int  GetMaxLevel();

    // ���úͻ�ȡ�Ƿ�����Ļ����ʾ��־��Ĭ��Ϊ����ʾ
    void SetPrintToScreen(bool printToScreen = true);
    bool GetPrintToScreen();

    // �����Ϣ�޸�������Ϣ�ᰴ���޸�������ӵ�˳�������޸�
    void AddMsgModifier(MsgModifier* msgFilter);

    // ��¼��־
    // msgΪ��־��Ϣ
    // levelΪ��־����Ĭ��ΪINFO����
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

