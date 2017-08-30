#ifndef __IJZ_IMAGESMOOTHPROC_H__
#define __IJZ_IMAGESMOOTHPROC_H__
#include <IJZImageSmooth.h>
#include <JZAutoLoader.h>

#define JZ_IMAGESMOOTH_DLL_NAME			_T("JZImageSmooth")
#define JZ_IMAGESMOOTH_API_EXT_NAME		"JZIS_GetAPIStuPtr"
#define JZ_IMAGESMOOTH_THIRD_PARTY		{ _T("OpenCV"), _T("OpenGL") }
#define JZ_IMAGESMOOTH_THIRD_PARTY_NUM	2


class CJZImageSmoothDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_IMAGESMOOTH_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_IMAGESMOOTH_API_EXT_NAME; }
	int ThirdPartyNum()const { return JZ_IMAGESMOOTH_THIRD_PARTY_NUM; }
	LPCTSTR* ThirdParty() const
	{
		LPCTSTR thirdParty[JZ_IMAGESMOOTH_THIRD_PARTY_NUM] = JZ_IMAGESMOOTH_THIRD_PARTY;
		return  thirdParty;
	}
};

__declspec(selectany) JZLoader<JZImageSmoothAPI, CJZImageSmoothDLLPos> g_JZImageSmoothAPI;  // 单键导出的接口
#endif // !__IJZ_IMAGESMOOTHPROC_H__
