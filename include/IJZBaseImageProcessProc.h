#ifndef __IJZ_BASEIMAGEPROCESSPROC_H__
#define __IJZ_BASEIMAGEPROCESSPROC_H__
#include <IJZBaseImageProcess.h>
#include <JZAutoLoader.h>

#define JZ_BASEIMAGEPROCESS_DLL_NAME			_T("JZBaseImageProcess")
#define JZ_BASEIMAGEPROCESS_API_EXT_NAME		"JZBIP_GetAPIStuPtr"
#define JZ_BASEIMAGEPROCESS_THIRD_PARTY			{ _T("OpenCV"), _T("OpenGL") }
#define JZ_BASEIMAGEPROCESS_THIRD_PARTY_NUM		2

// 加载器相对路径类
class CJZBaseImageProcessDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_BASEIMAGEPROCESS_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_BASEIMAGEPROCESS_API_EXT_NAME; }
	int ThirdPartyNum()const { return JZ_BASEIMAGEPROCESS_THIRD_PARTY_NUM; }
	LPCTSTR* ThirdParty() const 
	{ 
		LPCTSTR thirdParty[JZ_BASEIMAGEPROCESS_THIRD_PARTY_NUM] = JZ_BASEIMAGEPROCESS_THIRD_PARTY;
		return  thirdParty;
	}
};

//__declspec(selectany) JZAutoSetEnv g_BaseImageAutoSetEnv(_T("OpenCV"));	// 自动将依赖的第三方库设置为环境变量
__declspec(selectany) JZLoader<JZBaseImageProcessAPI, CJZBaseImageProcessDLLPos> g_JZBaseImageProcessAPI;  // 单键导出的接口

#endif // !__IJZ_BASEIMAGEPROCESSPROC_H__

