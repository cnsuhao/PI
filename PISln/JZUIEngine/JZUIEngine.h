#ifndef __JZ_UIENGINE_H__
#define __JZ_UIENGINE_H__
#include <IJZBaseRender.h>
#include <IJZUIEngine.h>
#include <IJZBaseImageProcess.h>
#include <map>
using namespace std;

class JZUIEngine: public IJZUIEngine
{
public:
	// ���캯��
	JZUIEngine();
	// ��������
	~JZUIEngine();
	// ��ʼ��UIEngine�г�Ա
	virtual JZ_RESULT Init(HWND hWnd);
	// �ͷ�UIEngine�г�Ա
	virtual JZ_RESULT Release();
	// ��ʼ��ͼ����Ⱦ�ӿ�
	virtual JZ_RESULT InitRenderSDK(HWND hWnd);
	// �ͷ�ͼ����Ⱦ�ӿ�
	virtual JZ_RESULT ReleaseRenderSDK();
	// ��ʼ��ͼ�������ӿ�
	virtual JZ_RESULT InitBaseImageSDK();
	// �ͷ�ͼ�������ӿ�
	virtual JZ_RESULT ReleaseBaseImageSDK();
	// ��ʼ��ͼ�������ӿ�
	virtual JZ_RESULT InitImageProcessPlugin();
	// �ͷ�ͼ�������ӿ�
	virtual JZ_RESULT ReleaseImageProcessPlugin();
	// ����Ҫ�����ͼ������
	virtual JZ_RESULT SetImageData(const char* filename);
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImageData();
	// ����ͼ����Ĳ���
	virtual JZ_RESULT SetProcessParam(JZCommonParam param, JZ_IMAGEPROC_TYPE eImageProcType);
	// ����ͼ�������ͽ���ͼ����
	virtual JZ_RESULT ProcessImage(JZ_IMAGEPROC_TYPE eImageProcType);
	// ��Ⱦ����
	virtual JZ_RESULT Render();
	

private:
	void _ReleaseImageProcessAPI(JZ_IMAGEPROC_TYPE eImageProcType, IJZBaseImageProcess*& pBaseImageProcess);

private:
	IJZSceneRender*									m_pSceneRender;			// ͼ����Ⱦ����
	JZImageProcessData								m_imageProcessData;		// ����ͼ�����ͼ������
	map<JZ_IMAGEPROC_TYPE, IJZBaseImageProcess*>	m_mapImageProcess;		// ��Ų�ͬͼ�������Ľӿ�  <ͼ��������-ͼ����ӿ�>
	map<JZ_IMAGEPROC_TYPE, JZCommonParam*>			m_mapProcessParam;		// ��Ų�ͬͼ�������Ĳ���  <ͼ��������-ͼ�������>
	IJZBaseImageProcess*							m_pBaseImageProcess;	// ͼ�������ӿ�
};

#endif // __JZ_UIENGINE_H__
