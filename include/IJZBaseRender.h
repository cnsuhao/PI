#ifndef __IJZ_BASERENDER_H__
#define __IJZ_BASERENDER_H__
#include <JZCommonImageDef.h>
#include <vector>
#include <glm/glm.hpp>
#include <Windows.h>

enum JZResType
{
	JZ_RES_UNKNOW,
	JZ_RES_DEVICE,			// OpenGL设备资源
	JZ_RES_SHADER,			// OpenGL shader资源
	JZ_RES_TEX,				// OpenGL 纹理资源
	JZ_RES_MESH,			// OpenGL网格资源
};

enum JZ_DRAW_UNIT
{
	JZ_DRAW_POINTS,			// 对应OpenGL中GL_POINTS
	JZ_DRAW_LINES,			// 对应OpenGL中GL_LINES
	JZ_DRAW_TRIANGLES,		// 对应OpenGL中GL_TRIANGLES
};

enum JZ_DRAW_METHOD
{
	JZ_DRAW_ARRAYS,			// 对应OpenGL中glDrawArrays()函数
	JZ_DRAW_ELEMENTS,		// 对应OpenGL中glDrawElements()函数
};

struct Vertex
{
	glm::vec3 position;		// 顶点坐标
	glm::vec3 color;		// 顶点颜色
	glm::vec2 texture;		// 顶点纹理坐标
};

// 资源基类
class IJZBaseRes
{
public:
	// 获取资源类型
	virtual JZResType GetResType() = 0;	
};

class IJZBaseRender
{
public:
	IJZBaseRender() {}
	~IJZBaseRender() {}
};

// OpenGL设备资源
class IJZDevice : public IJZBaseRes
{
public:
	// 获取资源类型
	virtual JZResType GetResType() = 0;
	// 创建设备资源
	virtual JZ_RESULT Create(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL) = 0;
	// 释放设备资源
	virtual JZ_RESULT Release() = 0;
	// 使用(绑定)设备
	virtual JZ_RESULT Use() = 0;
	// 解除绑定
	virtual JZ_RESULT UnUse() = 0;
};

// OpenGL的shader资源
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
	virtual JZ_RESULT Create(const char** shaderPath, int iShaderNums) = 0;
	// 获取着色器程序ID
	virtual unsigned int GetProgramID() = 0;
	// 释放着色器程序
	virtual void Release() = 0;
};

// OpenGL纹理资源
class IJZTexture: public IJZBaseRes
{
public:
	// 获取资源类型
	virtual JZResType GetResType() = 0;
	// 创建纹理资源，目前仅支持RGB图像
	virtual JZ_RESULT Create(JZImageBuf* pImageBuf) = 0;	
	// 释放纹理资源
	virtual JZ_RESULT Release() = 0;
	// 往纹理中填充图像数据，目前只支持和已经创建的纹理宽高一样的情况，该接口慎用
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf) = 0;	
	// 获取该纹理ID
	virtual unsigned int GetTexID() = 0;		
	// 获取纹理宽度
	virtual int GetTexWidth() = 0;
	// 获取纹理高度
	virtual int GetTexHeight() = 0;	
};

// OpenGL网格资源
class IJZMesh: public IJZBaseRes
{
public:
	// 获取资源类型
	virtual JZResType GetResType() = 0;
	// 创建网格资源
	virtual JZ_RESULT Create(std::vector<Vertex> vertexSet, std::vector<unsigned int> indexSet, JZ_DRAW_UNIT drawUnit, JZ_DRAW_METHOD drawMethod) = 0;
	// 创建用于显示图像的网格资源
	virtual JZ_RESULT CreateQuadMesh() = 0;
	// 释放网格资源
	virtual JZ_RESULT Release() = 0;
	// 绘制网格
	virtual JZ_RESULT Draw() = 0;
};

//
class IJZScene: public IJZBaseRender
{
public:
	// 设置OpenGL设备
	virtual JZ_RESULT SetDevice(HWND hWnd) = 0;		
	// 设置要渲染的图像， 仅支持RGB
	virtual void SetImage(JZImageBuf* pImageBuf) = 0;		
	// 准备绘制数据
	virtual void PrepareData() = 0;	
	// 设置资源的状态是否有改变
	virtual void SetResStatus(bool bHasChanged = true) = 0;
	// 渲染绘制图形
	virtual void RenderScene() = 0;
	// 释放内存
	virtual void Release() = 0;
};

typedef JZ_RESULT(*DefGetSceneInterface)(IJZScene** ppScene);
typedef JZ_RESULT(*DefReleaseSceneInterface)(IJZScene* pScene);

struct JZBaseRenderAPI
{
	DefGetSceneInterface pfnGetSceneInterface;
	DefReleaseSceneInterface pfnReleaseSceneInterface;
};

#endif // !__IJZ_BASERENDER_H__

