#ifndef __JZ_IMAGESMOOTH_H__
#define __JZ_IMAGESMOOTH_H__
#include <IJZImageSmooth.h>
class JZImageSmooth : public JZBaseImageProcess
{
public:
	// ͼ��ƽ��(ģ��)
	virtual JZ_RESULT ImageSmooth(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// ����ͼ��
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};

#endif // !__IJZ_IMAGESMOOTH_H__

