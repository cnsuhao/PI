#ifndef SCENE_H
#define SCENE_H
#include "IJZBaseRender.h"

class JZScene: public IJZScene
{
public:
	// 构造函数
	JZScene();	
	// 析构函数
	~JZScene();	
	// 设置OpenGL设备
	virtual JZ_RESULT SetDevice(HWND hWnd);
	// 设置要渲染的图像
	virtual void SetImage(JZImageBuf* pImageBuf);
	// 设置背景颜色
	void SetGroundColor(glm::vec4 color);	
	// 准备绘制数据
	virtual void PrepareData();
	// 设置资源的状态是否有改变
	virtual void SetResStatus(bool bHasChanged = true);
	// 渲染绘制图形
	virtual void RenderScene();
	// 释放内存
	virtual void Release();

private:
	glm::vec4		m_groundColor;		// 显示区域背景颜色RGBA
	IJZShader*		m_pShader;			// OpenGL绘制需要的shader资源
	IJZTexture*		m_pTexture;			// OpenGL绘制需要的纹理资源
	IJZDevice*		m_pDevice;			// OpenGL绘制需要的设备资源
	IJZMesh*		m_pMesh;			// OpenGL绘制需要的网格资源
	unsigned int	m_srcFramebuffer;	// 用于离屏渲染的帧缓存
	unsigned int	m_dstFramebuffer;
	bool			m_bResChanged;		// 用来标志是否有资源发生了改变，若改变了则要重新将数据传入显存
};

#endif