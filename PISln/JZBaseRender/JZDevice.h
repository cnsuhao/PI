#ifndef __JZ_DEVICE_H__
#define __JZ_DEVICE_H__
#include <IJZBaseRender.h>
class JZDevice: public IJZDevice
{
public:
	JZDevice();
	JZDevice(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL);
	~JZDevice();
	virtual JZResType GetResType();
	virtual JZ_RESULT CreateDevice(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL);
	virtual JZ_RESULT Use();
	virtual JZ_RESULT UnUse();
	virtual JZ_RESULT Release();

private:
	HDC m_hDC;						// ���ݴ��ڻ�ؼ��õ����豸�����ģ�����ͼ�λ���
	HGLRC m_hGLRC;					// OpenGL��Ⱦ����Ҫ���豸 
};


#endif // !__JZ_DEVICE_H__

