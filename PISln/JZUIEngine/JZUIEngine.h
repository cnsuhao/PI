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
	// ����Ҫ�����ͼ������
	virtual JZ_RESULT SetImageData(const char* filename);
	// �Ƿ��Ѿ�������Ҫ�����ͼ��
	virtual bool IsSetSrcImage();
	// ��������ͼ�����ݱ���ΪͼƬ
	virtual JZ_RESULT SaveImageData(const char* filename);
	// ����ͼ����Ĳ���
	virtual JZ_RESULT SetProcessParam(JZCommonParam* param);
	// ���õ�ǰͼ��������
	virtual JZ_RESULT SetCurProcessType(JZ_IMAGEPROC_TYPE curProcessType);
	// ����ͼ�������ͽ���ͼ����
	virtual JZ_RESULT ProcessImage();
	// ��ȡ�洢�Ķ���ͼ����������
	virtual JZImageProcessExtraData* GetExtraData();
	// ��Ⱦ����
	virtual JZ_RESULT Render();
	

private:
	// ��ʼ��ͼ����Ⱦ�ӿ�
	JZ_RESULT _InitRenderSDK(HWND hWnd);
	// �ͷ�ͼ����Ⱦ�ӿ�
	JZ_RESULT _ReleaseRenderSDK();
	// ��ʼ��ͼ�������ӿ�
	JZ_RESULT _InitBaseImageSDK();
	// �ͷ�ͼ�������ӿ�
	JZ_RESULT _ReleaseBaseImageSDK();
	// ��ʼ��ͼ�������ӿ�
	JZ_RESULT _InitImageProcessPlugin();
	// �ͷ�ͼ�������ӿ�
	JZ_RESULT _ReleaseImageProcessPlugin();
	// �ͷ�ͼ������
	JZ_RESULT _ReleaseImageData();
	// ����ͼ�������ͣ��ͷŶ�Ӧ��ͼ����ӿ�
	void _ReleaseImageProcessAPI(JZ_IMAGEPROC_TYPE eImageProcType, IJZBaseImageProcess*& pBaseImageProcess);
private:
	IJZSceneRender*									m_pSceneRender;			// ͼ����Ⱦ����
	JZImageProcessData								m_imageProcessData;		// ����ͼ�����ͼ������
	IJZBaseImageProcess*							m_pBaseImageProcess;	// ͼ�������ӿ�
	map<JZ_IMAGEPROC_TYPE, IJZBaseImageProcess*>	m_mapImageProcess;		// ��Ų�ͬͼ�������Ľӿ�  <ͼ��������-ͼ����ӿ�>
	map<JZ_IMAGEPROC_TYPE, JZCommonParam*>			m_mapProcessParam;		// ��Ų�ͬͼ�������Ĳ���  <ͼ��������-ͼ�������>
	JZ_IMAGEPROC_TYPE								m_curPocessType;		// ��ǰ��ͼ��������
	bool											m_bIsInit;				// ��־��ǰ�����Ƿ��Ѿ���ʼ����
};

#endif // __JZ_UIENGINE_H__
