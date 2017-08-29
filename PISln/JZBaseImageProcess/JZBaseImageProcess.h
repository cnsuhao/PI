#ifndef __JZ_BASEIMAGEPROCESS_H__
#define __JZ_BASEIMAGEPROCESS_H__

#include "IJZBaseImageProcess.h"

class JZBaseImageProcess : public IJZBaseImageProcess
{
public:
	// ���캯��
	JZBaseImageProcess();
	// ��������
	~JZBaseImageProcess();
	// дͼ��
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// ͼ��ƽ��(ģ��)
	virtual JZ_RESULT BlurImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage);

};
#endif // !__JZ_BASEIMAGEPROCESS_H__

