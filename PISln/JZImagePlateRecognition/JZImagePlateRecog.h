#ifndef __JZ_IMAGEPLATERECOG_H__
#define __JZ_IMAGEPLATERECOG_H__
#include <IJZImagePlateRecog.h>
class JZImagePlateRecog : public JZBaseImageProcess
{
public:
	// ´¦ÀíÍ¼Ïñ
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};


#endif // !__JZ_IMAGEPLATERECOG_H__

