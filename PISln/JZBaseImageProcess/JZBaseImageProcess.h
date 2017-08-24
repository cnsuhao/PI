#ifndef __JZ_BASEIMAGEPROCESS_H__
#define __JZ_BASEIMAGEPROCESS_H__

#include "IJZBaseImageProcess.h"

class JZBaseImageProcess : public IJZBaseImageProcessAPI
{
public:
	JZBaseImageProcess() {}
	~JZBaseImageProcess() {}
	JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  JZImageBuf, JZCommonParam* param);
	JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);

};
#endif // !__JZ_BASEIMAGEPROCESS_H__

