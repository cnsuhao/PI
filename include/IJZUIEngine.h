#ifndef __IJZ_UIENGINE_H__
#define __IJZ_UIENGINE_H__
#include <JZCommonImageDef.h>
#include <Windows.h>

class IJZUIEngine
{
public:
	// ��ʼ��UIEngine�г�Ա
	virtual JZ_RESULT Init(HWND hWnd) = 0;
	// �ͷ�UIEngine�г�Ա
	virtual JZ_RESULT Release() = 0;
	// ����Ҫ�����ͼ������
	virtual JZ_RESULT SetImageData(const char* filename) = 0;
	// �Ƿ��Ѿ�������Ҫ�����ͼ��
	virtual bool IsSetSrcImage() = 0;
	// ��������ͼ�����ݱ���ΪͼƬ
	virtual JZ_RESULT SaveImageData(const char* filename) = 0;
	// ���õ�ǰͼ��������
	virtual JZ_RESULT SetCurProcessType(JZ_IMAGEPROC_TYPE curProcessType) = 0;
	// ����ͼ����Ĳ���
	virtual JZ_RESULT SetProcessParam(JZCommonParam* param) = 0;
	// ����ͼ�������ͽ���ͼ����
	virtual JZ_RESULT ProcessImage() = 0;
	// ��Ⱦ����
	virtual JZ_RESULT Render() = 0;

};

typedef JZ_RESULT(*DefGetUIEngineInterface)(IJZUIEngine** ppAPI);
typedef JZ_RESULT(*DefReleaseUIEngineInterface)(IJZUIEngine*& pAPI);

struct JZUIEngineAPI
{
	DefGetUIEngineInterface pfnGetInterface;
	DefReleaseUIEngineInterface pfnReleaseInterface;
};
#endif // __IJZ_UIENGINE_H__

