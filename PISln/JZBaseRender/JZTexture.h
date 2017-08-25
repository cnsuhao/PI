#ifndef __JZ_TEXTURE_H__
#define __JZ_TEXTURE_H__
#include <IJZBaseRender.h>

class JZTexture: public IJZTexture
{
public:
	// ���캯��
	JZTexture();
	// ��������
	~JZTexture();
	// ��ȡ��Դ����
	virtual JZResType GetResType();		
	// ����������Դ��Ŀǰ��֧��RGBͼ��
	virtual JZ_RESULT Create(JZImageBuf* pImageBuf);	
	// �ͷ�������Դ
	virtual JZ_RESULT Release();	
	// �����������ͼ�����ݣ���֧��RGBͼ�񣬲���ֻ֧�ֺ��Ѿ�������������һ����������ýӿ�����
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf);	
	// ��ȡ������ID
	virtual unsigned int GetTexID();		
	// ��ȡ������
	virtual int GetTexWidth();	
	// ��ȡ����߶�
	virtual int GetTexHeight();
private:
	unsigned int m_texID;		// ����ID
	int m_texWidth;				// ������
	int m_texHeight;			// ����߶�
};



#endif // !__JZ_TEXTURE_H__

