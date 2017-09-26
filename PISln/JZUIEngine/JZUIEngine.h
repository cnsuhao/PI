#ifndef __JZ_UIENGINE_H__
#define __JZ_UIENGINE_H__
#include <IJZBaseRender.h>
#include <IJZUIEngine.h>
#include <IJZBaseImageProcess.h>
#include <IJZImagePlateRecog.h>
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
	// ����Ҫ�����ͼ������
	virtual JZ_RESULT SetImageData(const char* filename);
	// �Ƿ��Ѿ�������Ҫ�����ͼ��
	virtual bool IsSetSrcImage();
	// ��������ͼ�����ݱ���ΪͼƬ
	virtual JZ_RESULT SaveImageData(const char* filename);
	// ����ͼ����Ĳ���
	virtual JZ_RESULT SetProcessParam(JZCommonParam* param);
	// ����ͼ�������ͽ���ͼ����
	virtual JZ_RESULT ProcessImage(JZ_IMAGEPROC_TYPE processType, JZ_BASEPROCESS_TYPE baseProcessType = JZ_BASEPROCESS_UNKNOWN);
	// ��ȡ�洢�Ķ���ͼ����������
	virtual JZImageProcessExtraData* GetExtraData();
	// ��Ⱦ����
	virtual JZ_RESULT Render();
	

private:
	// ��ʼ��ͼ����Ⱦ�ӿ�
	JZ_RESULT _InitRenderSDK(HWND hWnd);
	// ��ʼ��ͼ����ӿںͲ���
	JZ_RESULT _InitImageProcessSDK();
	// �ͷ�ͼ����Ⱦ�ӿ�
	JZ_RESULT _ReleaseRenderSDK();
	// �ͷ�ͼ�������ӿںͲ���
	JZ_RESULT _ReleaseImageProcessSDK();
	// �ͷ�ͼ������
	JZ_RESULT _ReleaseImageData();
private:
	IJZSceneRender*									m_pSceneRender;			// ͼ����Ⱦ����
	JZImageProcessData								m_imageProcessData;		// ����ͼ�����ͼ������
	IJZBaseImageProcess*							m_pBaseImageProcess;	// ����ͼ����ӿ�
	map<JZ_BASEPROCESS_TYPE, JZCommonParam*>		m_mapBaseProcessParam;	// ����ͼ�������
	IJZImagePlateRecog*								m_pImagePlateRecog;		// ����ʶ��ӿ�
	JZCommonParam*									m_pPlateRecogParam;		// ����ʶ�����
	bool											m_bIsInit;				// ��־��ǰ�����Ƿ��Ѿ���ʼ����
};

#endif // __JZ_UIENGINE_H__
