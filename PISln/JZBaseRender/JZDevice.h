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
	HDC m_hDC;						// 根据窗口或控件得到的设备上下文，用于图形绘制
	HGLRC m_hGLRC;					// OpenGL渲染所需要的设备 
};


#endif // !__JZ_DEVICE_H__

