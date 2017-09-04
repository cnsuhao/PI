#ifndef __IJZ_IMAGEMORPHOLOGY_H__
#define __IJZ_IMAGEMORPHOLOGY_H__

#include <IJZBaseImageProcess.h>

// 供外部调用的接口声明
struct JZImageMorphologyAPI
{
	DefGetBaseImageInterface pfnGetInterface;
	DefReleaseBaseImageInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGEMORPHOLOGY_H__

