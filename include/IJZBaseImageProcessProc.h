#ifndef __IJZ_BASEIMAGEPROCESSPROC_H__
#define __IJZ_BASEIMAGEPROCESSPROC_H__
#include <IJZBaseImageProcess.h>
#include <JZAutoLoader.h>

#define JZ_BASEIMAGEPROCESS_DLL_NAME			_T("JZBaseImageProcess")
#define JZ_BASEIMAGEPROCESS_API_EXT_NAME		"JZBIP_GetAPIStuPtr"

// 加载器相对路径类
class CJZBaseImageProcessDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_BASEIMAGEPROCESS_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_BASEIMAGEPROCESS_API_EXT_NAME; }
};

__declspec(selectany) JZLoader<JZBaseImageProcessAPI, CJZBaseImageProcessDLLPos> g_JZBaseImageProcessAPI;  // 单键导出的接口

#endif // !__IJZ_BASEIMAGEPROCESSPROC_H__

