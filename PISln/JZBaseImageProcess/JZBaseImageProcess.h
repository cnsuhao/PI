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
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// дͼ��
	JZ_RESULT WriteImage(JZImageBuf* pImageBuf, const char* filename);
	// �ͷ�ͼ������
	JZ_RESULT ReleaseImage(JZImageBuf* pImage);
	// ����ͼ��
	JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);

private:
	// ͼ����̬ѧ����
	JZ_RESULT _ImageMorphology(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
	// ͼ��ƽ������
	JZ_RESULT _ImageSmooth(JZImageProcessData* pImageProcessData, JZCommonParam* pParam);
};
#endif // !__JZ_BASEIMAGEPROCESS_H__

