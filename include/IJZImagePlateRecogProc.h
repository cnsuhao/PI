#ifndef __IJZ_IMAGEPLATERECOGPROC_H__
#define __IJZ_IMAGEPLATERECOGPROC_H__

#include <IJZImagePlateRecog.h>
#include <JZAutoLoader.h>

#define JZ_IMAGEPLATERECOG_DLL_NAME			_T("JZImagePlateRecognition")
#define JZ_IMAGEPLATERECOG_API_EXT_NAME		"JZIPR_GetAPIStuPtr"
#define JZ_IMAGEPLATERECOG_THIRD_PARTY		{ _T("OpenCV"), _T("EasyPR") }
#define JZ_IMAGEPLATERECOG_THIRD_PARTY_NUM	2


class CJZImagePlateRecogDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_IMAGEPLATERECOG_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_IMAGEPLATERECOG_API_EXT_NAME; }
	int ThirdPartyNum()const { return JZ_IMAGEPLATERECOG_THIRD_PARTY_NUM; }
	LPCTSTR* ThirdParty() const
	{
		LPCTSTR thirdParty[JZ_IMAGEPLATERECOG_THIRD_PARTY_NUM] = JZ_IMAGEPLATERECOG_THIRD_PARTY;
		return  thirdParty;
	}
};

__declspec(selectany) JZLoader<JZImagePlateRecogAPI, CJZImagePlateRecogDLLPos> g_JZImagePlateRecogAPI;  // 单键导出的接口

#endif // !__IJZ_IMAGEPLATERECOGPROC_H__

