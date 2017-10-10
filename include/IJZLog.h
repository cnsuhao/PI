#ifndef __IJZ_LOG_H__
#define __IJZ_LOG_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonDef.h>

// ��־�����Ͷ���
enum JZ_LOG_TYPE
{
	JZ_LOG_TYPE_MESSAGE = 0,		// ��Ϣ
	JZ_LOG_TYPE_WARNING,			// ����
	JZ_LOG_TYPE_ERROR,			// ����
	JZ_LOG_TYPE_CRASH,			// ����

	JZ_LOG_TYPE_COUNT
};

// ��־��¼
typedef void(*DefWriteLog)(IN JZ_LOG_TYPE eLogType,		// ��־������
	IN LPCTSTR szFileName,			// ������־���ļ�
	IN LPCTSTR szFuncName,			// ������־�ĺ���
	IN LPCTSTR szFormat, ...);		// ��־����
typedef void(*DefWriteLogString)(IN JZ_LOG_TYPE eLogType,		// ��־������
	IN LPCTSTR szFileName,			// ������־���ļ�
	IN LPCTSTR szFuncName,			// ������־�ĺ���
	IN char const* szContent);			// ��־����

// ���ⲿ���õĽӿ�����
struct JZLogAPI
{
	DefWriteLog pfnWriteLog;
	DefWriteLogString pfnWriteLogString;
};

#endif // !__IJZ_LOG_H__
