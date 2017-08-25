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
	JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  JZImageBuf, JZCommonParam* param);
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage);

};
#endif // !__JZ_BASEIMAGEPROCESS_H__

