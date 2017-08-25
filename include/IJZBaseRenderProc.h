#ifndef __IJZ_BASERENDERAPI_H__
#define __IJZ_BASERENDERAPI_H__
#include <IJZBaseRender.h>
#include <JZAutoLoader.h>

#define JZ_BASERENDER_DLL_NAME			_T("JZBaseRender")
#define JZ_BASERENDER_API_EXT_NAME		"JZBR_GetAPIStuPtr"
#define JZ_BASERENDER_THIRD_PARTY		{_T("OpenGL")}
#define JZ_BASERENDER_THIRD_PARTY_NUM	1

class JZBaseRenderDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_BASERENDER_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_BASERENDER_API_EXT_NAME; }
	int ThirdPartyNum() const { return JZ_BASERENDER_THIRD_PARTY_NUM; }
	LPCTSTR* ThirdParty() const
	{
		LPCTSTR thirdParty[JZ_BASERENDER_THIRD_PARTY_NUM] = JZ_BASERENDER_THIRD_PARTY;
		return  thirdParty;
	}
};

//__declspec(selectany) JZAutoSetEnv g_BaseRenderAutoSetEnv(_T("OpenGL"));	// 自动将依赖的第三方库设置为环境变量
__declspec(selectany) JZLoader<JZBaseRenderAPI, JZBaseRenderDLLPos> g_JZBaseRenderAPI;  // 单键导出的接口

#endif // !__IJZ_BASERENDERAPI_H__

