#ifndef __IJZ_LOGPROC_H__
#define __IJZ_LOGPROC_H__
#include <IJZLog.h>
#include <JZAutoLoader.h>

#define JZ_LOG_DLL_NAME			_T("JZLog")
#define JZ_LOG_API_EXT_NAME		"JZLog_GetAPIStuPtr"
#define JZ_LOG_THIRD_PARTY			{}
#define JZ_LOG_THIRD_PARTY_NUM		0

// 加载器相对路径类
class CJZLogDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_LOG_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_LOG_API_EXT_NAME; }
	int ThirdPartyNum()const { return JZ_LOG_THIRD_PARTY_NUM; }
	LPCTSTR* ThirdParty() const
	{
		return  NULL;
	}
};

__declspec(selectany) JZLoader<JZLogAPI, CJZLogDLLPos> g_JZLogAPI;  // 单键导出的接口

#endif // !__IJZ_LOGPROC_H__

