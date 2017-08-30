#ifndef __IJZ_IMAGESMOOTH_H__
#define __IJZ_IMAGESMOOTH_H__
#include <IJZBaseImageProcess.h>

typedef JZ_RESULT(*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);

// ���ⲿ���õĽӿ�����
struct JZImageSmoothAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGESMOOTH_H__
