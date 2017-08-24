#ifndef __JZ_DEVICE_H__
#define __JZ_DEVICE_H__
#include <IJZBaseRender.h>

class JZDevice: public IJZDevice
{
public:
	// 构造函数
	JZDevice();
	// 重载构造函数
	JZDevice(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL);
	// 析构函数
	~JZDevice();
	// 获取资源类型
	virtual JZResType GetResType();
	// 创建设备资源
	virtual JZ_RESULT Create(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL);
	// 释放设备资源
	virtual JZ_RESULT Release();
	// 使用(绑定)设备
	virtual JZ_RESULT Use();
	// 解除绑定
	virtual JZ_RESULT UnUse();
	

private:
	HDC m_hDC;						// 根据窗口或控件得到的设备上下文，用于图形绘制
	HGLRC m_hGLRC;					// OpenGL渲染所需要的设备 
};


#endif // !__JZ_DEVICE_H__

