#ifndef __IJZ_IMAGEPLATERECOG_H__
#define __IJZ_IMAGEPLATERECOG_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>
// �ӿڻ���
class IJZImagePlateRecog
{
public:
	// ����ͼ��
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
};


typedef JZ_RESULT(*DefGetImagePlateRecogInterface)(IJZImagePlateRecog** ppAPI);
typedef JZ_RESULT(*DefReleaseImagePlateRecogInterface)(IJZImagePlateRecog*& pAPI);

// ���ⲿ���õĽӿ�����
struct JZImagePlateRecogAPI
{
	DefGetImagePlateRecogInterface pfnGetInterface;
	DefReleaseImagePlateRecogInterface pfnReleaseInterface;
};

#endif // !__IJZ_IMAGEPLATERECOG_H__

