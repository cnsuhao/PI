#ifndef __IJZ_IMAGEPLATERECOG_H__
#define __IJZ_IMAGEPLATERECOG_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>
// 接口基类
class IJZImagePlateRecog
{
public:
	// 处理图像
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
};


typedef JZ_RESULT(*DefGetImagePlateRecogInterface)(IJZImagePlateRecog** ppAPI);
typedef JZ_RESULT(*DefReleaseImagePlateRecogInterface)(IJZImagePlateRecog*& pAPI);

// 供外部调用的接口声明
struct JZImagePlateRecogAPI
{
	DefGetImagePlateRecogInterface pfnGetInterface;
	DefReleaseImagePlateRecogInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGEPLATERECOG_H__

