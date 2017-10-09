//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: CPU SDK ��־�ļ�����.
//
// Created: ZhaoZhimeng @ 2009-04-04
//////////////////////////////////////////////////////////////////////////
#ifndef _NX_CPULOG_H__
#define _NX_CPULOG_H__

#ifdef JZLOG_BUILD
#define JZLOG_API __declspec(dllexport)
#else
#define JZLOG_API __declspec(dllimport)
#endif

// ��־�����Ͷ���
enum NXCPU_LOG_TYPE
{
	NXCPU_LOG_TYPE_MESSAGE = 0,		// ��Ϣ
	NXCPU_LOG_TYPE_WARNING,			// ����
	NXCPU_LOG_TYPE_ERROR,			// ����
	NXCPU_LOG_TYPE_CRASH,			// ����

	NXCPU_LOG_TYPE_COUNT
};

// ���ò���
struct NXCoreConfig
{
	int		log;				// ��־�������
	int		log_level;			// ��־�������
	int		num_of_threads;		// ���߳��Ż���
	int		mt_resampling;		// �ز����ӿڶ��߳��Ż�
	int		mt_deinterlacing;	// ȥ��֯�ӿڶ��߳��Ż�
};

// ������־ϵͳ
bool InitLogFile(HMODULE hModule);
// �ر���־ϵͳ
void ReleaseLogFile();
class CNXCPULog
{
public:
	CNXCPULog(void);
	~CNXCPULog(void);

public:
	// ����־�ļ�
	// ��1���Զ�д��ʽ�򿪣������־�ļ��Ѵ�����򿪣��������򴴽���
	bool OpenFile(IN LPCTSTR szFilePath);	// ��־�ļ�·��
	// �ж���־�ļ��Ƿ��
	bool IsOpened();
	// �ر���־�ļ�
	bool CloseFile();
	// д��һ����־��
	bool WriteLogLine(IN LPCTSTR szLogInfo);
	// ������־�ļ�·��
	bool SetFilePath(IN LPCTSTR szFilePath);
	// ��ȡ��־�ļ�·��
	LPCTSTR GetFilePath() const;

protected:
	// ��ȡϵͳʱ�䣬Ҫ��������ִ���������Ϊ 20��
	bool _GetSysTimeStr(OUT LPTSTR szTime);
	// ����־�ļ������д��һ���ִ�
	void _WriteStrAtEnd(IN LPCTSTR szInfo);

protected:
	enum
	{
		STR_TIME_SIZE = 40,
	};

protected:
	HANDLE				m_hLogFile;		// ��־�ļ����
	CRITICAL_SECTION	m_csFile;		// ��־�ļ��Ļ�����
	TCHAR				m_szStart[STR_TIME_SIZE];	// ��־�ļ��򿪵�ʱ��
	TCHAR				m_szFilePath[MAX_PATH];		// ��־�ļ�·��
};

// inline ʵ��

// �ж���־�ļ��Ƿ��
inline bool CNXCPULog::IsOpened()
{
	bool bOpened = false;

	EnterCriticalSection(&m_csFile);
	bOpened = (m_hLogFile != NULL);
	LeaveCriticalSection(&m_csFile);

	return bOpened;
}

// ��ȡ��־�ļ�·��
inline LPCTSTR CNXCPULog::GetFilePath() const
{
	return m_szFilePath;
}

// ��־��¼
JZLOG_API void NCB_WriteLog(IN NXCPU_LOG_TYPE eLogType,		// ��־������
	IN LPCTSTR szFileName,			// ������־���ļ�
	IN LPCTSTR szFuncName,			// ������־�ĺ���
	IN LPCTSTR szFormat, ...);		// ��־����

JZLOG_API void NCB_WriteLogString(IN NXCPU_LOG_TYPE eLogType,		// ��־������
	IN LPCTSTR szFileName,			// ������־���ļ�
	IN LPCTSTR szFuncName,			// ������־�ĺ���
	IN char const* szContent);		// ��־����


#endif	// _NX_CPULOG_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
