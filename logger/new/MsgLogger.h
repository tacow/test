#ifndef MSG_LOGGER_H
#define MSG_LOGGER_H

#include <string>
#include "MsgQueue.h"

using namespace std;

#define MAX_MSG_LEN  (1024 * 1024)

class IMsg {
public:
    // ����Ϣ�ṹ��ת��Ϊ�ַ������ַ���ĩβ������"\n"����������"\0"
    // bufΪ�ṩ���ַ�����������bufSizeΪ��������󳤶ȣ���־������ض�
    // ����ֵΪ�ַ�������
    virtual size_t ToStr(char* buf, size_t bufSize) = 0;

    // �ͷ���Ϣ�ṹ��
    virtual void Release() = 0;
};

class MsgLogger {
public:
    MsgLogger();
    ~MsgLogger();

    // ��ʼ����־
    // prefixΪ��־�ļ���ǰ׺��
    //     * ��ǰ����д����־�ļ���������Ϊ[prefix].log
    //     * ��ʷ��־�ļ���������Ϊ[prefix]_[yyyymmddHHMMSS].log���ļ����е�����ʱ�������־���з�ʱ��ʱ��
    // logPathΪ��־��ŵ�·����Ĭ��Ϊ��ǰĿ¼
    // maxFileLenΪ��־�ļ���󳤶ȣ������ó������Զ��з֣�Ĭ��Ϊ100M
    void Init(const char* prefix, const char* logPath = ".", int maxFileLen = 104857600); 

    // �ر���־
    void Close();

    // ���úͻ�ȡ����Ļ�������־����
    // 0�����������Ļ��1���������stdout��2���������stderr��Ĭ��Ϊ�����
    int GetPrintToScreen();
    void SetPrintToScreen(int printToScreen = 1);

    // �����־
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

