#ifndef __JZ_IMAGESMOOTH_H__
#define __JZ_IMAGESMOOTH_H__
#include <IJZImageSmooth.h>
class JZImageSmooth : public JZBaseImageProcess
{
public:
	// 图像平滑(模糊)
	virtual JZ_RESULT ImageSmooth(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// 处理图像
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};

#endif // !__IJZ_IMAGESMOOTH_H__

