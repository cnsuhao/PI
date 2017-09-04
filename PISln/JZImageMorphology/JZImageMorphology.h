#ifndef __JZ_IMAGEMORPHOLOGY_H__
#define __JZ_IMAGEMORPHOLOGY_H__
#include <IJZImageMorphology.h>
class JZImageMorphology : public JZBaseImageProcess
{
public:
	// ´¦ÀíÍ¼Ïñ
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};


#endif // !__JZ_IMAGEMORPHOLOGY_H__

