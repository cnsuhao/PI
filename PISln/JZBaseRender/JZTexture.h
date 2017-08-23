#ifndef __JZ_TEXTURE_H__
#define __JZ_TEXTURE_H__
#include <IJZBaseRender.h>
#include <GL/glew.h> // ����glew����ȡ���еı���OpenGLͷ�ļ�
#include <vector>
class JZTexture
{
public:
	JZTexture();
	~JZTexture();
	JZ_RESULT Create(GLsizei width, GLsizei height);	// ����������Դ
	JZ_RESULT Release();	// �ͷ�������Դ
	JZ_RESULT FillImage(JZImageBuf* pImageBuf);	// �����������ͼ������
	GLuint GetTexID();		// ��ȡ������ID
	GLsizei GetTexWidth();	// ��ȡ������
	GLsizei GetTexHeight();	// ��ȡ����߶�
private:
	GLuint m_texID;
	GLsizei m_texWidth;
	GLsizei m_texHeight;
};

class JZTexManager
{
public:
	JZTexManager();
	~JZTexManager();
	JZTexture* CreateTexture();
	JZ_RESULT ReleaseTexture(JZTexture* pTexture);

private:
	std::vector<GLuint> m_vecTexID; // �Ѿ������Ĳ���û�б��ͷŵ�������Դ��ID
};


#endif // !__JZ_TEXTURE_H__

