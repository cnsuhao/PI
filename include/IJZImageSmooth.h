#ifndef __IJZ_IMAGESMOOTH_H__
#define __IJZ_IMAGESMOOTH_H__
#include <IJZImageProcessBase.h>

// ���ⲿ���õĽӿ�����
struct JZImageSmoothAPI
{
	DefGetImageBaseInterface pfnGetInterface;
	DefReleaseImageBaseInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGESMOOTH_H__
