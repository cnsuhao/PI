#ifndef SCENE_H
#define SCENE_H
#include "IJZBaseRender.h"

class JZScene: public IJZSceneRender
{
public:
	// 构造函数
	JZScene();	
	// 析构函数
	~JZScene();	
	// 设置OpenGL设备
	virtual JZ_RESULT SetDevice(HWND hWnd);
	// 设置shader资源
	virtual void SetShader();
	// 设置要渲染的左侧图像， 仅支持RGB
	virtual void SetLeftImage(JZImageBuf* pImageBuf = NULL);
	// 设置要渲染的右侧图像， 仅支持RGB
	virtual void SetRightImage(JZImageBuf* pImageBuf = NULL);
	// 设置背景颜色
	virtual void SetGroundColor(glm::vec4 color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
	// 初始化渲染需要的基本数据
	virtual void init(HWND hWnd);
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
	HWND			m_hwnd;				// 用于绘制的窗口区域
	bool			m_bResChanged;		// 用来标志是否有资源发生了改变，若改变了则要重新将数据传入显存
};

#endif