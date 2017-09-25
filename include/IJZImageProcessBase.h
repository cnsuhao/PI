#ifndef __IJZ_IMAGEPROCESSBASE_H__
#define __IJZ_IMAGEPROCESSBASE_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

// 接口基类
class IJZImageProcessBase
{
public:
	// 处理图像
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
};


typedef JZ_RESULT(*DefGetImageBaseInterface)(IJZImageProcessBase** ppAPI);
typedef JZ_RESULT(*DefReleaseImageBaseInterface)(IJZImageProcessBase*& pAPI);


#endif
