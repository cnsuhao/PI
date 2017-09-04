#ifndef __IJZ_IMAGEMORPHOLOGYPROC_H__
#define __IJZ_IMAGEMORPHOLOGYPROC_H__

#include <IJZImageMorphology.h>
#include <JZAutoLoader.h>

#define JZ_IMAGEMORPHOLOGY_DLL_NAME			_T("JZImageMorphology")
#define JZ_IMAGEMORPHOLOGY_API_EXT_NAME		"JZIM_GetAPIStuPtr"
#define JZ_IMAGEMORPHOLOGY_THIRD_PARTY		{ _T("OpenCV") }
#define JZ_IMAGEMORPHOLOGY_THIRD_PARTY_NUM	1


class CJZImageMorphologyDLLPos
{
public:
	LPCTSTR DLLName() const { return JZ_IMAGEMORPHOLOGY_DLL_NAME; }
	LPCSTR APIExtName() const { return JZ_IMAGEMORPHOLOGY_API_EXT_NAME; }
	int ThirdPartyNum()const { return JZ_IMAGEMORPHOLOGY_THIRD_PARTY_NUM; }
	LPCTSTR* ThirdParty() const
	{
		LPCTSTR thirdParty[JZ_IMAGEMORPHOLOGY_THIRD_PARTY_NUM] = JZ_IMAGEMORPHOLOGY_THIRD_PARTY;
		return  thirdParty;
	}
};

__declspec(selectany) JZLoader<JZImageMorphologyAPI, CJZImageMorphologyDLLPos> g_JZImageMorphologyAPI;  // 单键导出的接口

#endif // !__IJZ_IMAGEMORPHOLOGYPROC_H__

