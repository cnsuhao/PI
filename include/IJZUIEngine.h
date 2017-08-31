#ifndef __IJZ_UIENGINE_H__
#define __IJZ_UIENGINE_H__
#include <JZCommonImageDef.h>
#include <Windows.h>

enum JZ_IMAGEPROC_TYPE
{
	JZ_IMAGE_UNKONW = -1,
	JZ_IMAGE_SMOOTH,
};

class IJZUIEngine
{
public:
	// ��ʼ��UIEngine�г�Ա
	virtual JZ_RESULT Init(HWND hWnd) = 0;
	// �ͷ�UIEngine�г�Ա
	virtual JZ_RESULT Release() = 0;
	// ��ʼ��ͼ����Ⱦ�ӿ�
	virtual JZ_RESULT InitRenderSDK(HWND hWnd) = 0;
	// �ͷ�ͼ����Ⱦ�ӿ�
	virtual JZ_RESULT ReleaseRenderSDK() = 0;
	// ��ʼ��ͼ�������ӿ�
	virtual JZ_RESULT InitBaseImageSDK() = 0;
	// �ͷ�ͼ�������ӿ�
	virtual JZ_RESULT ReleaseBaseImageSDK() = 0;
	// ��ʼ��ͼ�������ӿ�
	virtual JZ_RESULT InitImageProcessPlugin() = 0;
	// �ͷ�ͼ�������ӿ�
	virtual JZ_RESULT ReleaseImageProcessPlugin() = 0;
	// ����Ҫ�����ͼ������
	virtual JZ_RESULT SetImageData(const char* filename) = 0;
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImageData() = 0;
	// ����ͼ����Ĳ���
	virtual JZ_RESULT SetProcessParam(JZCommonParam param, JZ_IMAGEPROC_TYPE eImageProcType) = 0;
	// ����ͼ�������ͽ���ͼ����
	virtual JZ_RESULT ProcessImage(JZ_IMAGEPROC_TYPE eImageProcType) = 0;
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

