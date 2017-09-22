#ifndef __IJZ_IMAGEPLATERECOG_H__
#define __IJZ_IMAGEPLATERECOG_H__

#include <IJZBaseImageProcess.h>

// 供外部调用的接口声明
struct JZImagePlateRecogAPI
{
	DefGetBaseImageInterface pfnGetInterface;
	DefReleaseBaseImageInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGEPLATERECOG_H__

