#ifndef __IJZ_BASEIMAGEPROCESS_H__
#define __IJZ_BASEIMAGEPROCESS_H__

#include <Windows.h>
#include <tchar.h>
#include <JZLoader.h>

#define JZBASEIMAGEPROCESS_DLL_NAME			_T("JZBaseImageProcess")
#define JZBASEIMAGEPROCESS_API_EXT_NAME		"JZBIP_GetAPIStuPtr"

struct JZCommonParam
{

};

enum JZ_RESULT
{
	JZ_OK,
	JZ_FAILED,
	JZ_INVAILD_PARAM,
};

class IJZBaseImageProcess
{
public:
	IJZBaseImageProcess();
	~IJZBaseImageProcess();
	virtual JZ_RESULT ProcessImage(void* psrc, void*  pdes, JZCommonParam* param) = 0;
	JZ_RESULT ReadImage(const char* filename, void* pImage);
private:
	JZCommonParam* pParam;
};

typedef JZ_RESULT (*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);
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

__declspec(selectany) JZLoader<JZBaseImageProcessAPI, CJZBaseImageProcessDLLPos> g_JZBaseImageProcessAPI;
#endif
