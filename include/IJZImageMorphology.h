#ifndef __IJZ_IMAGEMORPHOLOGY_H__
#define __IJZ_IMAGEMORPHOLOGY_H__

#include <IJZImageProcessBase.h>

// 供外部调用的接口声明
struct JZImageMorphologyAPI
{
	DefGetImageBaseInterface pfnGetInterface;
	DefReleaseImageBaseInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGEMORPHOLOGY_H__

