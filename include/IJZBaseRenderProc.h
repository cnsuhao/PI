#ifndef __IJZ_BASERENDERAPI_H__
#define __IJZ_BASERENDERAPI_H__
#include <JZAutoLoader.h>
#include <IJZBaseRender.h>

#define JZ_BASERENDER_DLL_NAME			_T("JZBaseRender")
#define JZ_BASERENDER_API_EXT_NAME		"JZBR_GetAPIStuPtr"


class JZBaseRenderDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_BASERENDER_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_BASERENDER_API_EXT_NAME; }

};
__declspec(selectany) JZLoader<JZBaseRenderAPI, JZBaseRenderDLLPos> g_JZBaseRenderAPI;  // 单键导出的接口


#endif // !__IJZ_BASERENDERAPI_H__

