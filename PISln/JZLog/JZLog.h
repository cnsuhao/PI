//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: CPU SDK ��־�ļ�����.
//
// Created: ZhaoZhimeng @ 2009-04-04
//////////////////////////////////////////////////////////////////////////
#ifndef _NX_CPULOG_H__
#define _NX_CPULOG_H__
#include <IJZLog.h>


// ���ò���
struct JZLogConfig
{
	int		log;				// ��־�������
	int		log_level;			// ��־�������
	int		num_of_threads;		// ���߳��Ż���
	int		mt_resampling;		// �ز����ӿڶ��߳��Ż�
	int		mt_deinterlacing;	// ȥ��֯�ӿڶ��߳��Ż�
};

class JZLog
{
public:
	JZLog(void);
	~JZLog(void);

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
inline bool JZLog::IsOpened()
{
	bool bOpened = false;

	EnterCriticalSection(&m_csFile);
	bOpened = (m_hLogFile != NULL);
	LeaveCriticalSection(&m_csFile);

	return bOpened;
}

// ��ȡ��־�ļ�·��
inline LPCTSTR JZLog::GetFilePath() const
{
	return m_szFilePath;
}

#endif	// _NX_CPULOG_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
