#ifndef __IJZ_IMAGEPLATERECOG_H__
#define __IJZ_IMAGEPLATERECOG_H__

#include <IJZBaseImageProcess.h>

// ���ⲿ���õĽӿ�����
struct JZImagePlateRecogAPI
{
	DefGetBaseImageInterface pfnGetInterface;
	DefReleaseBaseImageInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGEPLATERECOG_H__

