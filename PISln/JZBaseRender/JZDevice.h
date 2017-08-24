#ifndef __JZ_DEVICE_H__
#define __JZ_DEVICE_H__
#include <IJZBaseRender.h>

class JZDevice: public IJZDevice
{
public:
	// ���캯��
	JZDevice();
	// ���ع��캯��
	JZDevice(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL);
	// ��������
	~JZDevice();
	// ��ȡ��Դ����
	virtual JZResType GetResType();
	// �����豸��Դ
	virtual JZ_RESULT Create(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL);
	// �ͷ��豸��Դ
	virtual JZ_RESULT Release();
	// ʹ��(��)�豸
	virtual JZ_RESULT Use();
	// �����
	virtual JZ_RESULT UnUse();
	

private:
	HDC m_hDC;						// ���ݴ��ڻ�ؼ��õ����豸�����ģ�����ͼ�λ���
	HGLRC m_hGLRC;					// OpenGL��Ⱦ����Ҫ���豸 
};


#endif // !__JZ_DEVICE_H__

