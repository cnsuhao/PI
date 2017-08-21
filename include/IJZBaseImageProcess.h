#ifndef __IJZ_BASEIMAGEPROCESS_H__
#define __IJZ_BASEIMAGEPROCESS_H__

#include <Windows.h>
#include <tchar.h>
#include <JZAutoLoader.h>
#include <JZCommonImageDef.h>

#define JZBASEIMAGEPROCESS_DLL_NAME			_T("JZBaseImageProcess")
#define JZBASEIMAGEPROCESS_API_EXT_NAME		"JZBIP_GetAPIStuPtr"

// 接口基类
class IJZBaseImageProcess
{
public:
	IJZBaseImageProcess() {}
	~IJZBaseImageProcess() {}
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
};

typedef JZ_RESULT (*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);

// 供外部调用的接口声明
struct JZBaseImageProcessAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};

// 加载器相对路径类
class CJZBaseImageProcessDLLPos
{
public:
	LPCTSTR DLLName() const { return JZBASEIMAGEPROCESS_DLL_NAME; }
	LPCSTR APIExtName() const { return JZBASEIMAGEPROCESS_API_EXT_NAME; }
};

__declspec(selectany) JZLoader<JZBaseImageProcessAPI, CJZBaseImageProcessDLLPos> g_JZBaseImageProcessAPI;  // 单键导出的接口
#endif
