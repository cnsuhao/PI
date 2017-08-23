#ifndef __IJZ_BASERENDER_H__
#define __IJZ_BASERENDER_H__
#include <JZCommonImageDef.h>
#include <Windows.h>
enum JZResType
{
	JZ_RES_DEVICE,
	JZ_RES_SHADER,
	JZ_RES_TEX,
};
class IJZBaseRes
{
public:
	IJZBaseRes() {}
	~IJZBaseRes() {}
	virtual JZResType GetResType() = 0;
};

class IJZBaseRender
{
public:
	IJZBaseRender() {}
	~IJZBaseRender() {}
	virtual JZ_RESULT InitOpenGL() = 0;

private:

};

class IJZDevice : public IJZBaseRes
{
public:
	virtual JZResType GetResType() = 0;
	virtual JZ_RESULT CreateDevice(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL) = 0;
	virtual JZ_RESULT Use() = 0;
	virtual JZ_RESULT UnUse() = 0;
	virtual JZ_RESULT Release() = 0;
};

class IJZShader: public IJZBaseRes
{
public:
	// 获取资源类型
	virtual JZResType GetResType() = 0;
	// 使用着色器程序
	virtual void Use() = 0;
	// 解除使用着色器程序
	virtual void UnUse() = 0;
	// shaderPath表示包含了shader路径的字符串数组，iShaderNums表示该字符串数组包含了几个shader路径，字符串数组应该按照顶点着色器、片段着色器、几何着色器的顺序排列
	virtual JZ_RESULT CreateShaderProgram(const char** shaderPath, int iShaderNums) = 0;
	// 获取着色器程序ID
	virtual unsigned int GetProgramID() = 0;
	// 释放着色器程序
	virtual void Release() = 0;
};

class IJZTexture: public IJZBaseRes
{
public:
	virtual JZResType GetResType() = 0;
	virtual JZ_RESULT Create(int width, int height, JZImageBuf* pImageBuf = NULL) = 0;	// 创建纹理资源，目前仅支持RBG得buffer
	virtual JZ_RESULT Release() = 0;	// 释放纹理资源
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf) = 0;	// 往纹理中填充图像数据，目前仅支持RBG得buffer
	virtual unsigned int GetTexID() = 0;		// 获取该纹理ID
	virtual int GetTexWidth() = 0;	// 获取纹理宽度
	virtual int GetTexHeight() = 0;	// 获取纹理高度
};

class IJZScene: public IJZBaseRender
{
public:
	virtual void SetHWnd(HWND cwnd) = 0;
	virtual JZ_RESULT InitOpenGL() = 0;
	virtual void PrepareData() = 0;
	virtual void PushDataToGPU() = 0;
	virtual void RenderScene() = 0;
};

#endif // !__IJZ_BASERENDER_H__

