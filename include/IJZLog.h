#ifndef __IJZ_LOG_H__
#define __IJZ_LOG_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

// �ӿڻ���
class IJZLog
{
public:
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
	// дͼ��
	virtual JZ_RESULT WriteImage(JZImageBuf* pImageBuf, const char* filename) = 0;
	// ����ͼ��
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage) = 0;
};

typedef JZ_RESULT(*DefGetBaseImageInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseBaseImageInterface)(IJZBaseImageProcess*& pAPI);

// ���ⲿ���õĽӿ�����
struct JZBaseImageProcessAPI
{
	DefGetBaseImageInterface pfnGetInterface;
	DefReleaseBaseImageInterface pfnReleaseInterface;
};

#endif // !__IJZ_LOG_H__
