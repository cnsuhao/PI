#ifndef __JZ_TEXTURE_H__
#define __JZ_TEXTURE_H__
#include <IJZBaseRender.h>

class JZTexture: public IJZTexture
{
public:
	JZTexture();
	~JZTexture();
	virtual JZResType GetResType();		// ��ȡ��Դ����
	virtual JZ_RESULT Create(int width, int height, JZImageBuf* pImageBuf = NULL);	// ����������Դ
	virtual JZ_RESULT Release();	// �ͷ�������Դ
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf);	// �����������ͼ������
	virtual unsigned int GetTexID();		// ��ȡ������ID
	virtual int GetTexWidth();	// ��ȡ������
	virtual int GetTexHeight();	// ��ȡ����߶�
private:
	unsigned int m_texID;
	int m_texWidth;
	int m_texHeight;
};



#endif // !__JZ_TEXTURE_H__

