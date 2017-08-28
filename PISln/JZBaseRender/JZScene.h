#ifndef SCENE_H
#define SCENE_H
#include "IJZBaseRender.h"

class JZScene: public IJZScene
{
public:
	// ���캯��
	JZScene();	
	// ��������
	~JZScene();	
	// ����OpenGL�豸
	virtual JZ_RESULT SetDevice(HWND hWnd);
	// ����Ҫ��Ⱦ��ͼ��
	virtual void SetImage(JZImageBuf* pImageBuf);
	// ���ñ�����ɫ
	void SetGroundColor(glm::vec4 color);	
	// ׼����������
	virtual void PrepareData();
	// ������Դ��״̬�Ƿ��иı�
	virtual void SetResStatus(bool bHasChanged = true);
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
	bool			m_bResChanged;		// ������־�Ƿ�����Դ�����˸ı䣬���ı�����Ҫ���½����ݴ����Դ�
};

#endif