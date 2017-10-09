//////////////////////////////////////////////////////////////////////////
// Copyright (c) Sobey Corporation.  All rights reserved.
//
// Description: CPU SDK ��־�ļ�����.
//
// Created: ZhaoZhimeng @ 2009-04-04
//////////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <tchar.h>
#include <time.h>
#include <io.h>
#include <Shlwapi.h>
#define JZLOG_BUILD
#include "JZLog.h"
#pragma comment(lib, "Shlwapi.lib")
#define NXCPU_LOG_NAME_FMT			_T("CoreSDK-%04d%02d%02d-%02d%02d%02d.txt")
#define NXCPU_LOG_FILE_DIR			_T("\\CoreLog\\")

CNXCPULog*		g_pCPULog = NULL;
NXCoreConfig	g_CoreConf;

// ��־�����̣߳�ɾ��ʱ��ϳ�����ʷ��־����ֹ�ļ�����
ULONG __stdcall thread_log_manager(void* _Param)
{
	// ʱ����Ϊһ��
	int const time_interval = 7;
	time_t t = time(NULL);
	t -= time_interval * 24 * 3600;
	struct tm* lt = localtime(&t);
	if (lt == NULL)
		return (ULONG)0;
	lt->tm_year += 1900;
	lt->tm_mon++;
	TCHAR filename[MAX_PATH];
	filename[0] = _T('\0');
	_stprintf(filename, NXCPU_LOG_NAME_FMT, lt->tm_year, lt->tm_mon,
		lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
#ifdef _WIN32
	_tfinddata_t _File;
	intptr_t _Hfile;
	LPTSTR filespec = _tcsrchr((LPTSTR)_Param, _T('\\'));
	if (filespec == NULL)
		return (ULONG)0;
	filespec++;
	_tcscpy(filespec, _T("*.txt"));
	if ((_Hfile = _tfindfirst((LPCTSTR)_Param, &_File)) != -1)
	{
		filespec = _tcsrchr((LPTSTR)_Param, _T('\\')) + 1;
		do
		{
			if (_tcscmp(_File.name, filename) <= 0)
			{
				_tcscpy(filespec, _File.name);
				_tremove((LPCTSTR)_Param);
			}
		} while (!_tfindnext(_Hfile, &_File));
	}
	_findclose(_Hfile);
#endif
	return (ULONG)0;
}

CNXCPULog::CNXCPULog(void)
{
	m_hLogFile = NULL;
	InitializeCriticalSection(&m_csFile);
	m_szStart[0] = _T('\0');
	m_szFilePath[0] = _T('\0');
}

CNXCPULog::~CNXCPULog(void)
{
	CloseFile();
	DeleteCriticalSection(&m_csFile);
}

// ����־�ļ�
// ��1���Զ�д��ʽ�򿪣������־�ļ��Ѵ�����򿪣��������򴴽���
bool CNXCPULog::OpenFile(IN LPCTSTR szFilePath)		// ��־�ļ�·��
{
	EnterCriticalSection(&m_csFile);
	if (szFilePath == NULL || IsOpened())
		return false;

	// �Զ�д��ʽ���ļ����������򴴽�
#ifdef _WIN32
	m_hLogFile = CreateFile(szFilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ,
		NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL|FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	if (m_hLogFile == NULL || m_hLogFile == INVALID_HANDLE_VALUE)
		return false;
#else
	m_hLogFile = fopen(szFilePath, "w");
#endif

	// ��¼��־�ļ���ʱ�䣬��д��򿪱�־
#ifdef _WIN32
#ifdef _UNICODE	// ȷ���ļ��� UNICODE �汾
	ULONG ulHigh(0);

	if (GetFileSize(m_hLogFile, &ulHigh) == 0 && ulHigh == 0)	// �ļ��մ���
	{
		unsigned char ucUnicode[2] = { 0xff, 0xfe };
		ULONG ulSize = 2;

		WriteFile(m_hLogFile, ucUnicode, ulSize, &ulHigh, NULL);
	}
#endif
#endif
	m_szStart[0] = _T('\0');
	_GetSysTimeStr(m_szStart);
	_WriteStrAtEnd(_T("\r\n#### Log Start - "));
	_WriteStrAtEnd(m_szStart);
	_WriteStrAtEnd(_T(" ####\r\n"));
	LeaveCriticalSection(&m_csFile);
	return true;
}

// �ر���־�ļ�
bool CNXCPULog::CloseFile()
{
	if (IsOpened())
	{
		TCHAR szStop[STR_TIME_SIZE] = { _T('\0') };

		EnterCriticalSection(&m_csFile);
		_GetSysTimeStr(szStop);
		_WriteStrAtEnd(_T("#### Log Stop - "));
		_WriteStrAtEnd(szStop);
		_WriteStrAtEnd(_T(" ####\r\n"));
#ifdef _WIN32
		CloseHandle(m_hLogFile);
#else
		fclose((FILE *)m_hLogFile);
#endif
		m_hLogFile = NULL;
		LeaveCriticalSection(&m_csFile);
	}
	return true;
}

// д��һ����־��
bool CNXCPULog::WriteLogLine(IN LPCTSTR szLogInfo)
{
	if (!IsOpened())
		return false;

	// �����־�ļ���С�������ƣ���ɾ�������´�������ֹ��־�ļ�����
#ifdef _WIN32
	EnterCriticalSection(&m_csFile);
	if(GetFileSize(m_hLogFile, NULL) > (1 << 20))
	{
		CloseFile();
		OpenFile(GetFilePath());
	}
	LeaveCriticalSection(&m_csFile);
#endif

	TCHAR szTime[STR_TIME_SIZE] = { _T('\0') };

	EnterCriticalSection(&m_csFile);
	_GetSysTimeStr(szTime);
	_WriteStrAtEnd(_T("## "));
	_WriteStrAtEnd(szTime);
	_WriteStrAtEnd(_T("    "));
	_WriteStrAtEnd(szLogInfo);
	_WriteStrAtEnd(_T(" ##\r\n"));
	LeaveCriticalSection(&m_csFile);

	return true;
}

// ��ȡϵͳʱ�䣬Ҫ��������ִ���������Ϊ STR_TIME_SIZE
bool CNXCPULog::_GetSysTimeStr(OUT LPTSTR szTime)
{
	if (szTime == NULL)
		return false;

	int nLen(0);

#ifdef _WIN32
	_tstrdate(szTime);
	nLen = (int)_tcslen(szTime);
	szTime[nLen] = _T(' ');	// ���м�д��ո�
	_tstrtime(szTime + nLen + 1);
#else
	time_t t = time(NULL);
	char* str = ctime(&t);
	nLen = (int)strlen(str);
	memmove(szTime, str, nLen);
	szTime[nLen - 1] = '\0';
#endif

	return true;
}

// ����־�ļ������д��һ���ִ�
void CNXCPULog::_WriteStrAtEnd(IN LPCTSTR szInfo)
{
	if (szInfo == NULL)
		return;

	ULONG ulSize = (ULONG)(_tcslen(szInfo) * sizeof(TCHAR));
	ULONG ulWrite(0);

	if (ulSize != 0)
	{
#ifdef _WIN32
		::SetFilePointer(m_hLogFile, 0, NULL, FILE_END);
		::WriteFile(m_hLogFile, szInfo, ulSize, &ulWrite, NULL);
#else
		fseek((FILE *)m_hLogFile, 0, SEEK_END);
		ulWrite = (ULONG)fwrite(szInfo, 1, ulSize, (FILE *)m_hLogFile);
#endif
	}
}

// ������־�ļ�·��
bool CNXCPULog::SetFilePath(IN LPCTSTR szFilePath)
{
	if (_tcslen(szFilePath) < MAX_PATH)
	{
		_tcscpy(m_szFilePath, szFilePath);
		return true;
	}
	return false;
}

// ��־��¼
JZLOG_API void NCB_WriteLog(IN NXCPU_LOG_TYPE eLogType,		// ��־������
	IN LPCTSTR szFileName,			// ������־���ļ�
	IN LPCTSTR szFuncName,			// ������־�ĺ���
	IN LPCTSTR szFormat, ...)		// ��־����
{
	// ������ò������־��ֱ�ӷ���
	if (g_pCPULog == NULL || g_CoreConf.log == 0 || eLogType < g_CoreConf.log_level)
		return;

	// ���δ������־�ļ������ȴ���
	if (!g_pCPULog->IsOpened())
	{
		g_pCPULog->OpenFile(g_pCPULog->GetFilePath());
	}

	const int NXLOG_CONTENT_SIZE = 512;
	TCHAR szLogContent[NXLOG_CONTENT_SIZE];
	TCHAR szLogLine[(NXLOG_CONTENT_SIZE << 1)];
#ifdef __linux__
	va_list vl;
#else
	va_list vl = { NULL };
#endif

	szLogContent[0] = szLogLine[0] = _T('\0');
	// ������־����
	va_start(vl, szFormat);
	_vsntprintf(szLogContent, NXLOG_CONTENT_SIZE, szFormat, vl);
	va_end(vl);
	// �����־���ʽΪ LogType    FileName    FuncName    Content
	switch (eLogType)		// �������־������
	{
	case NXCPU_LOG_TYPE_MESSAGE:	// ��Ϣ
		_tcscat(szLogLine, _T("<Message>    "));
		break;
	case NXCPU_LOG_TYPE_WARNING:	// ����
		_tcscat(szLogLine, _T("<Warning>    "));
		break;
	case NXCPU_LOG_TYPE_ERROR:		// ����
		_tcscat(szLogLine, _T("<Error!!>    "));
		break;
	case NXCPU_LOG_TYPE_CRASH:		// ����
		_tcscat(szLogLine, _T("<!!Crash>    "));
		break;
	default:		// ����
		_tcscat(szLogLine, _T("<Unknown>    "));
		break;
	}
	// ����ļ�����
	if (szFileName != NULL)
	{
		_tcsncat(szLogLine, szFileName, ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
		_tcsncat(szLogLine, _T("    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	else
	{
		_tcsncat(szLogLine, _T("Unknown    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	// ��Ϻ�������
	if (szFuncName != NULL)
	{
		_tcsncat(szLogLine, szFuncName, ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
		_tcsncat(szLogLine, _T("    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	else
	{
		_tcsncat(szLogLine, _T("Unknown    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	// �����־����
	_tcsncat(szLogLine, szLogContent, ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	// д����־��
	g_pCPULog->WriteLogLine(szLogLine);
}

JZLOG_API void NCB_WriteLogString(IN NXCPU_LOG_TYPE eLogType,		// ��־������
	IN LPCTSTR szFileName,			// ������־���ļ�
	IN LPCTSTR szFuncName,			// ������־�ĺ���
	IN char const* szContent)			// ��־����
{
	// ������ò������־��ֱ�ӷ���
	if (g_pCPULog == NULL || g_CoreConf.log == 0 || eLogType < g_CoreConf.log_level || !szContent)
		return;

	// ���δ������־�ļ������ȴ���
	if (!g_pCPULog->IsOpened())
	{
		g_pCPULog->OpenFile(g_pCPULog->GetFilePath());
	}

	const int NXLOG_CONTENT_SIZE = 512;
	TCHAR szLogContent[NXLOG_CONTENT_SIZE];
	TCHAR szLogLine[(NXLOG_CONTENT_SIZE << 1)];
	szLogContent[0] = szLogLine[0] = _T('\0');

	// �����־���ʽΪ LogType    FileName    FuncName    Content
	switch (eLogType)		// �������־������
	{
	case NXCPU_LOG_TYPE_MESSAGE:	// ��Ϣ
		_tcscat(szLogLine, _T("<Message>    "));
		break;
	case NXCPU_LOG_TYPE_WARNING:	// ����
		_tcscat(szLogLine, _T("<Warning>    "));
		break;
	case NXCPU_LOG_TYPE_ERROR:		// ����
		_tcscat(szLogLine, _T("<Error!!>    "));
		break;
	case NXCPU_LOG_TYPE_CRASH:		// ����
		_tcscat(szLogLine, _T("<!!Crash>    "));
		break;
	default:		// ����
		_tcscat(szLogLine, _T("<Unknown>    "));
		break;
	}
	// ����ļ�����
	if (szFileName != NULL)
	{
		_tcsncat(szLogLine, szFileName, ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
		_tcsncat(szLogLine, _T("    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	else
	{
		_tcsncat(szLogLine, _T("Unknown    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	// ��Ϻ�������
	if (szFuncName != NULL)
	{
		_tcsncat(szLogLine, szFuncName, ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
		_tcsncat(szLogLine, _T("    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	else
	{
		_tcsncat(szLogLine, _T("Unknown    "), ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	}
	// �����־����
#if defined(_MSC_VER) && defined(UNICODE)
	MultiByteToWideChar(CP_ACP, 0, szContent, -1, szLogContent, NXLOG_CONTENT_SIZE);
#else
	_tcscpy(szLogContent, szContent);
#endif
	_tcsncat(szLogLine, szLogContent, ((size_t)(NXLOG_CONTENT_SIZE << 1) - _tcslen(szLogLine)));
	// д����־��
	g_pCPULog->WriteLogLine(szLogLine);
}

// ������־ϵͳ
bool InitLogFile(HMODULE hModule)
{
	if (g_pCPULog != NULL)
		return true;

#ifdef __APPLE__
	return true;
#endif

	try
	{
		int const MAX_FILE_PATH = 4096;
		static TCHAR szLogPath[MAX_FILE_PATH];		// ����·��
		szLogPath[MAX_FILE_PATH - 1] = _T('\0');
#ifdef _WIN32
		int i = 0;

		// ��ȡ��־�ļ�·�����ڵ�ǰִ��·���½�����
		if (GetModuleFileName(hModule, szLogPath, MAX_FILE_PATH - 1) != 0)
		{
			for (i = (int)_tcslen(szLogPath) - 1; i >= 0; i--)
			{
				if (szLogPath[i] == _T('\\'))
					break;
			}
			if (i <= 0)
			{
				szLogPath[0] = _T('.');
				i = 1;
			}
		}
		szLogPath[i] = _T('\0');
		// ��ȡ���ò���
		TCHAR* szConfig = new TCHAR[i + MAX_PATH];
		_tcscpy(szConfig, szLogPath);
		_tcscat(szConfig, _T("\\NXCoreSDKConf.ini"));
		if ((g_CoreConf.log = GetPrivateProfileInt(_T("Log"), _T("OutputLog"), -1, szConfig)) == -1)
		{
#ifdef _DEBUG
			g_CoreConf.log = 1;
			WritePrivateProfileString(_T("Log"), _T("OutputLog"), _T("1"), szConfig);
#else
			g_CoreConf.log = 0;
			WritePrivateProfileString(_T("Log"), _T("OutputLog"), _T("0"), szConfig);
#endif
		}
		if ((g_CoreConf.log_level = GetPrivateProfileInt(_T("Log"), _T("OutputLogLevel"), -1, szConfig)) == -1)
		{
#ifdef _DEBUG
			g_CoreConf.log_level = NXCPU_LOG_TYPE_MESSAGE;
			WritePrivateProfileString(_T("Log"), _T("OutputLogLevel"), _T("0    ; 0-MESSAGE, 1-WARNING, 2-ERROR, 3-CRASH"), szConfig);
#else
			g_CoreConf.log_level = NXCPU_LOG_TYPE_ERROR;
			WritePrivateProfileString(_T("Log"), _T("OutputLogLevel"), _T("2    ; 0-MESSAGE, 1-WARNING, 2-ERROR, 3-CRASH"), szConfig);
#endif
		}
		LPCTSTR szSection = _T("MultithreadedOptimization");
		if ((g_CoreConf.num_of_threads = GetPrivateProfileInt(szSection, _T("NumberOfThreads"), -1, szConfig)) == -1)
		{
			g_CoreConf.num_of_threads = 4;
			WritePrivateProfileString(szSection, _T("NumberOfThreads"), _T("4"), szConfig);
		}
		if ((g_CoreConf.mt_resampling = GetPrivateProfileInt(szSection, _T("Resampling"), -1, szConfig)) == -1)
		{
			g_CoreConf.mt_resampling = 0;
			WritePrivateProfileString(szSection, _T("Resampling"), _T("0"), szConfig);
		}
		if ((g_CoreConf.mt_deinterlacing = GetPrivateProfileInt(szSection, _T("Deinterlacing"), -1, szConfig)) == -1)
		{
			g_CoreConf.mt_deinterlacing = 0;
			WritePrivateProfileString(szSection, _T("Deinterlacing"), _T("0"), szConfig);
		}
		delete[] szConfig;
		// ������־�ļ���
		_tcscat(szLogPath, NXCPU_LOG_FILE_DIR);
#elif defined(__linux__)
		strcpy(szLogPath, "/mnt/sdcard/CoreSDK/");
		Dl_info dlinfo = { NULL };
		if (dladdr((void *)InitLogFile, &dlinfo) >= 0 && dlinfo.dli_fname != NULL)
		{
			_tcscpy(szLogPath, dlinfo.dli_fname);
			char* _Mydelimiter;
			if ((_Mydelimiter = strrchr(szLogPath, '/')) != NULL)
				*++_Mydelimiter = '\0';
		}
		std::string strlogpath(szLogPath);
		strlogpath += NXCPU_CONF_INI;
		std::ifstream fp(strlogpath.c_str());
		if (fp.is_open())
		{
			std::string strline;
			while (!fp.eof())
			{
				std::getline(fp, strline);
				if (strline.empty() || strline[0] == '[' || strline[0] == ';')
					continue;
				size_t p = strline.rfind(';');
				if (p != std::string::npos)
					strline.erase(p);
				p = strline.rfind('=');
				if (p == std::string::npos)
					continue;
				std::string strkey = strline.substr(0, p).substr(strline.find_first_not_of(' '));
				std::string strvalue = strline.substr(strline.find_first_not_of(' ', p + 1));
				if ((p = strkey.find_first_of(' ')) != std::string::npos)
					strkey.erase(p);
				if ((p = strvalue.find_first_of(' ')) != std::string::npos)
					strvalue.erase(p);
				p = atoi(strvalue.c_str());
				if (strkey == "OutputLog")
					g_CoreConf.log = p;
				else if (strkey == "OutputLogLevel")
					g_CoreConf.log_level = p;
				else if (strkey == "NumberOfThreads")
					g_CoreConf.num_of_threads = p;
				else if (strkey == "Deinterlacing")
					g_CoreConf.mt_deinterlacing = p;
				else if (strkey == "Resampling")
					g_CoreConf.mt_resampling = p;
			}
		}
		strcat(szLogPath, "CoreLog/");
#endif
		if (g_CoreConf.log == 0 && !PathFileExists(szLogPath))
			return true;
		if (!PathFileExists(szLogPath) && !CreateDirectory(szLogPath, NULL))
			return false;
		time_t t = time(NULL);
		struct tm* lt = localtime(&t);
		if (lt == NULL)
			return false;
		lt->tm_year += 1900;
		lt->tm_mon++;
		TCHAR filename[MAX_PATH];
		filename[MAX_PATH - 1] = _T('\0');
		_stprintf(filename, NXCPU_LOG_NAME_FMT, lt->tm_year, lt->tm_mon,
			lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
		_tcscat(szLogPath, filename);
		static CNXCPULog _Mystaticlog;
		g_pCPULog = &_Mystaticlog;
		// ������־�����߳�
		HANDLE _Thdlogmgr = CreateThread(NULL, 0, thread_log_manager, szLogPath, 0, NULL);
		CloseHandle(_Thdlogmgr);
		// ������־�ļ�·��
		g_pCPULog->SetFilePath(szLogPath);
	}
	catch (...)
	{
		ReleaseLogFile();
		return false;
	}
	return true;
}

// �ر���־ϵͳ
void ReleaseLogFile()
{
}

//////////////////////////////////////////////////////////////////////////
// End of file.
