#ifndef __IJZ_BASEIMAGEPROCESS_H__
#define __IJZ_BASEIMAGEPROCESS_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

// �ӿڻ���
class IJZBaseImageProcess
{
public:
	// дͼ��
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
	// ͼ��ƽ��(ģ��)
	virtual JZ_RESULT BlurImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage) = 0;
};

typedef JZ_RESULT(*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);

// ���ⲿ���õĽӿ�����
struct JZBaseImageProcessAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};


#endif
