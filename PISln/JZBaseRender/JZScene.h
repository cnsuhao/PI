#ifndef SCENE_H
#define SCENE_H
#include "IJZBaseRender.h"

class JZScene: public IJZSceneRender
{
public:
	// ���캯��
	JZScene();	
	// ��������
	~JZScene();	
	// ����OpenGL�豸
	virtual JZ_RESULT SetDevice(HWND hWnd);
	// ����shader��Դ
	virtual void SetShader();
	// ����Ҫ��Ⱦ�����ͼ�� ��֧��RGB
	virtual void SetLeftImage(JZImageBuf* pImageBuf = NULL);
	// ����Ҫ��Ⱦ���Ҳ�ͼ�� ��֧��RGB
	virtual void SetRightImage(JZImageBuf* pImageBuf = NULL);
	// ���ñ�����ɫ
	virtual void SetGroundColor(glm::vec4 color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
	// ��ʼ����Ⱦ��Ҫ�Ļ�������
	virtual void init(HWND hWnd);
	// ��Ⱦ����ͼ��
	virtual void RenderScene();
	// �ͷ��ڴ�
	virtual void Release();

private:
	glm::vec4		m_groundColor;		// ��ʾ���򱳾���ɫRGBA
	IJZShader*		m_pShader;			// OpenGL������Ҫ��shader��Դ
	IJZTexture*		m_pTexture;			// OpenGL������Ҫ��������Դ
	IJZDevice*		m_pDevice;			// OpenGL������Ҫ���豸��Դ
	IJZMesh*		m_pMesh;			// OpenGL������Ҫ��������Դ
	unsigned int	m_srcFramebuffer;	// ����������Ⱦ��֡����
	unsigned int	m_dstFramebuffer;
	HWND			m_hwnd;				// ���ڻ��ƵĴ�������
	bool			m_bResChanged;		// ������־�Ƿ�����Դ�����˸ı䣬���ı�����Ҫ���½����ݴ����Դ�
};

#endif