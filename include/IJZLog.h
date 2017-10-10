#ifndef __IJZ_LOG_H__
#define __IJZ_LOG_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonDef.h>

// 日志项类型定义
enum JZ_LOG_TYPE
{
	JZ_LOG_TYPE_MESSAGE = 0,		// 消息
	JZ_LOG_TYPE_WARNING,			// 警告
	JZ_LOG_TYPE_ERROR,			// 错误
	JZ_LOG_TYPE_CRASH,			// 崩溃

	JZ_LOG_TYPE_COUNT
};

// 日志记录
typedef void(*DefWriteLog)(IN JZ_LOG_TYPE eLogType,		// 日志项类型
	IN LPCTSTR szFileName,			// 发生日志的文件
	IN LPCTSTR szFuncName,			// 发生日志的函数
	IN LPCTSTR szFormat, ...);		// 日志内容
typedef void(*DefWriteLogString)(IN JZ_LOG_TYPE eLogType,		// 日志项类型
	IN LPCTSTR szFileName,			// 发生日志的文件
	IN LPCTSTR szFuncName,			// 发生日志的函数
	IN char const* szContent);			// 日志内容

// 供外部调用的接口声明
struct JZLogAPI
{
	DefWriteLog pfnWriteLog;
	DefWriteLogString pfnWriteLogString;
};

#endif // !__IJZ_LOG_H__
