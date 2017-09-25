#ifndef __IJZ_IMAGESMOOTH_H__
#define __IJZ_IMAGESMOOTH_H__
#include <IJZImageProcessBase.h>

// 供外部调用的接口声明
struct JZImageSmoothAPI
{
	DefGetImageBaseInterface pfnGetInterface;
	DefReleaseImageBaseInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGESMOOTH_H__
