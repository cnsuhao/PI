#ifndef __JZ_IMAGEMORPHOLOGY_H__
#define __JZ_IMAGEMORPHOLOGY_H__
#include <IJZImageMorphology.h>
class JZImageMorphology : public IJZImageProcessBase
{
public:
	// ����ͼ��
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};


#endif // !__JZ_IMAGEMORPHOLOGY_H__

