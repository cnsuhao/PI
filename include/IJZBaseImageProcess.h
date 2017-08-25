#ifndef __IJZ_BASEIMAGEPROCESS_H__
#define __IJZ_BASEIMAGEPROCESS_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

// 接口基类
class IJZBaseImageProcessAPI
{
public:
	IJZBaseImageProcessAPI() {}
	~IJZBaseImageProcessAPI() {}
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
};

typedef JZ_RESULT(*DefGetInterface)(IJZBaseImageProcessAPI** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcessAPI* pAPI);

// 供外部调用的接口声明
struct JZBaseImageProcessAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};


#endif
