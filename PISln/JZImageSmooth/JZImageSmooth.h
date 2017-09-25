#ifndef __JZ_IMAGESMOOTH_H__
#define __JZ_IMAGESMOOTH_H__
#include <IJZImageSmooth.h>
class JZImageSmooth : public IJZImageProcessBase
{
public:
	// ´¦ÀíÍ¼Ïñ
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};

#endif // !__IJZ_IMAGESMOOTH_H__

