#ifndef SCENE_H
#define SCENE_H

/////////////////头文件///////////////////////
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include "IJZBaseRender.h"
#include "camera.h"
#include <Windows.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
};

class JZScene: public IJZScene
{
public:
	/////////////////////////////【1】OpenGL初始化时用到的一些变量////////////////////////
	Camera camera;					// 相机参数
	HWND m_hwnd;					// MFC中控件的句柄，可用函数GetDlgItem()获取
	glm::vec4 groundColor;			// 显示区域背景颜色RGBA
	glm::vec3 pointColor;			// 绘制点的颜色RGB
	std::vector<Vertex> pointArray; // 存放用于绘制点的容器

	/////////////////////////////【2】绘制部分用到的变量
	const char** m_shaderPath;		// shader路径数组
	int m_shaderNums;				// shader路径数组中包含了几个着色器,按照顶点着色器、片段着色器、几何着色器的顺序排列
	IJZShader* m_pShader;			// OpenGL中的shader资源
	IJZTexture* m_pTexture;			// OpenGL中的纹理资源
	IJZDevice* m_pDevice;			// OpenGL设备资源

	GLuint m_VAO, m_VBO, m_EBO;		// OpenGL中VAO、VBO
	glm::mat4 modelMatrix;			// 模型矩阵
	glm::mat4 viewMatrix;			// 观察矩阵
	glm::mat4 projectionMatrix;		// 投影矩阵

	//////////////////////////// 【3】响应鼠标和按键用到的变量///////////////////////////////
	glm::vec2 downPoint;			// 按下鼠标左键时，鼠标所在点坐标
	glm::vec2 upPoint;				// 松开鼠标左键时，鼠标所在点坐标
	bool keys[1024];				// 键盘按键的状态，true表示按下，false表示松开
	float lastFrameTime;			// 上一帧的时刻
	float deltaTime;				// 本帧和上一帧的时间差

	glm::vec2 lastFramePoint;		// 上一帧鼠标指针所在点坐标
	bool firstCursor;				// 是否是鼠标第一次拖动时的位置
	bool isCSModel;					// 是否是CS模式

public:
	JZScene();	// 构造函数
	~JZScene();	// 析构函数

	/////////////////////////////1.初始化部分的函数///////////////////////////////////////
public:
	void SetHWnd(HWND cwnd);											// 设置窗口或控件句柄
	void SetGroundColor(glm::vec4 color);								// 设置背景颜色
	void SetPointColor(glm::vec3 color);								// 设置绘制点颜色
	void InitMatrix();													// 初始化变换矩阵
	void Reset();														// 复位视角
	void PrepareData();													// 准备绘制数据
	void PushDataToGPU();												// 把数据从内存传到显存
	JZ_RESULT InitOpenGL();												// 初始化OpenGL中内容

	/////////////////////////////2.绘制部分的函数////////////////////////////////////////
	void AddPoint(Vertex point);										// 向绘制点容器中添加绘制点
	void DeletePoint(glm::vec3 point);									// 从绘制点容器中删除点
	void RenderScene();													// 渲染绘制图形

	/////////////////////////////3.响应鼠标和按键部分的函数///////////////////////////////////
public:
	void SetDownPoint(glm::vec2 point);									// 设置鼠标左键按下时，鼠标所在点坐标
	void SetUpPoint(glm::vec2 point);									// 设置鼠标左键松开时，鼠标所在点坐标
	void SetKeyState(UINT nChar, bool state, bool resetAll = false);	// 设置键盘按键的状态
	void SwitchViewModel();												// 在CS模式和非CS模式之间切换
	void MouseControlView(POINT currentPoint);							// 通过鼠标控制视角，输入参数currentPoint是鼠标在客户区域的相对位置
	void MouseWheelControlProjection(short zDelta, POINT point);		// 滚轮改变投影矩阵

private:
	void _UpdateViewAndProjectionMatrix();								// 更新变换矩阵
	bool _IsCursorInRect();												// 判断鼠标指针是否在图形绘制区域内
	void _KeyControlCameraAndModel();									// 按住W、A、S、D时，相机视角进行相应改变
	void _MouseControlModel();											// 通过鼠标来控制模型旋转

	////////////////////////////4.释放内存部分的函数///////////////////////////////////
public:
	void DestroyOpenGL();												// 释放OpenGL相关

};

#endif