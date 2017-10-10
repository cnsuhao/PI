//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: CPU SDK 日志文件处理.
//
// Created: ZhaoZhimeng @ 2009-04-04
//////////////////////////////////////////////////////////////////////////
#ifndef _NX_CPULOG_H__
#define _NX_CPULOG_H__
#include <IJZLog.h>


// 配置参数
struct JZLogConfig
{
	int		log;				// 日志输出开关
	int		log_level;			// 日志输出级别
	int		num_of_threads;		// 多线程优化数
	int		mt_resampling;		// 重采样接口多线程优化
	int		mt_deinterlacing;	// 去交织接口多线程优化
};

class JZLog
{
public:
	JZLog(void);
	~JZLog(void);

public:
	// 打开日志文件
	// （1）以读写方式打开，如果日志文件已存在则打开，不存在则创建。
	bool OpenFile(IN LPCTSTR szFilePath);	// 日志文件路径
	// 判断日志文件是否打开
	bool IsOpened();
	// 关闭日志文件
	bool CloseFile();
	// 写入一个日志项
	bool WriteLogLine(IN LPCTSTR szLogInfo);
	// 设置日志文件路径
	bool SetFilePath(IN LPCTSTR szFilePath);
	// 获取日志文件路径
	LPCTSTR GetFilePath() const;

protected:
	// 获取系统时间，要求输入的字串容量最少为 20。
	bool _GetSysTimeStr(OUT LPTSTR szTime);
	// 在日志文件的最后写入一个字串
	void _WriteStrAtEnd(IN LPCTSTR szInfo);

protected:
	enum
	{
		STR_TIME_SIZE = 40,
	};

protected:
	HANDLE				m_hLogFile;		// 日志文件句柄
	CRITICAL_SECTION	m_csFile;		// 日志文件的互斥锁
	TCHAR				m_szStart[STR_TIME_SIZE];	// 日志文件打开的时间
	TCHAR				m_szFilePath[MAX_PATH];		// 日志文件路径
};

// inline 实现

// 判断日志文件是否打开
inline bool JZLog::IsOpened()
{
	bool bOpened = false;

	EnterCriticalSection(&m_csFile);
	bOpened = (m_hLogFile != NULL);
	LeaveCriticalSection(&m_csFile);

	return bOpened;
}

// 获取日志文件路径
inline LPCTSTR JZLog::GetFilePath() const
{
	return m_szFilePath;
}

#endif	// _NX_CPULOG_H__
//////////////////////////////////////////////////////////////////////////
// End of file.
