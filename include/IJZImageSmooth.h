#ifndef __IJZ_IMAGESMOOTH_H__
#define __IJZ_IMAGESMOOTH_H__
#include <IJZBaseImageProcess.h>

// ���ⲿ���õĽӿ�����
struct JZImageSmoothAPI
{
	DefGetBaseImageInterface pfnGetInterface;
	DefReleaseBaseImageInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGESMOOTH_H__
