#include "JZDevice.h"
#include <GL/glew.h>
#include <cassert>

#pragma comment(lib, "opengl32.lib")
#ifdef _DEBUG
#	ifdef _WIN64
#		pragma comment(lib, "glew64d.lib")
#	else
#		pragma comment(lib, "glew32d.lib")
#	endif
#else
#	ifdef _WIN64
#		pragma comment(lib, "glew64.lib")
#	else
#		pragma comment(lib, "glew32.lib")
#	endif
#endif // _DEBUG


JZDevice::JZDevice()
{
	m_hDC = NULL;
	m_hGLRC = NULL;
}

JZDevice::JZDevice(HWND hWnd, HDC hDC, HGLRC hShareRC)
{
	if (NULL == hWnd)
	{
		return ;
	}
	m_hDC = hDC;
	if (NULL == m_hDC)
	{
		m_hDC = ::GetDC(hWnd);
	}


	GLenum eRet = 0;
	static PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24 ,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32 ,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelformat;
	if (!(pixelformat = ChoosePixelFormat(m_hDC, &pfd)))	// 获取最匹配得像素格式
	{
		return ;
	}
	if (!SetPixelFormat(m_hDC, pixelformat, &pfd))	// 设置虚拟像素格式，方便对wgl函数访问
	{
		return ;
	}
	if (!(m_hGLRC = wglCreateContext(m_hDC)))	// 创建OpenGL渲染环境
	{
		return ;
	}
	if (!wglMakeCurrent(m_hDC, m_hGLRC))	// 设置为当前的渲染上下文环境
	{
		return ;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	RECT rect; //在这个矩形中画图  
	GetClientRect(hWnd, &rect);
	//绘制区域占据整个窗口大小 
	GLsizei width = rect.right - rect.left;
	GLsizei height = rect.bottom - rect.top;
	glViewport(0, 0, width, height);

	eRet = glGetError();
	assert(0 == eRet);
}

JZDevice::~JZDevice()
{
	Release();
}

JZ_RES_TYPE JZDevice::GetResType()
{
	return JZ_RES_DEVICE;
}

JZ_RESULT JZDevice::Create(HWND hWnd, HDC hDC/* = NULL*/, HGLRC hShareRC/* = NULL*/)
{
	if (NULL == hWnd)
	{
		return JZ_FAILED;
	}
	m_hDC = hDC;
	if (NULL == m_hDC)
	{
		m_hDC = ::GetDC(hWnd);
	}

	static PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24 ,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32 ,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelformat;
	if (!(pixelformat = ChoosePixelFormat(m_hDC, &pfd)))	// 获取最匹配得像素格式
	{
		return JZ_FAILED;
	}
	if (!SetPixelFormat(m_hDC, pixelformat, &pfd))	// 设置虚拟像素格式，方便对wgl函数访问
	{
		return JZ_FAILED;
	}
	if (!(m_hGLRC = wglCreateContext(m_hDC)))	// 创建OpenGL渲染环境
	{
		return JZ_FAILED;
	}
	if (!wglMakeCurrent(m_hDC, m_hGLRC))	// 设置为当前的渲染上下文环境
	{
		return JZ_FAILED;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	assert(0 == glGetError());

	return JZ_SUCCESS;
}

JZ_RESULT JZDevice::Use()
{
	if (NULL == m_hDC || NULL == m_hGLRC)
	{
		return JZ_FAILED;
	}

	if (!wglMakeCurrent(m_hDC, m_hGLRC))	// 设置为当前的渲染上下文环境
	{
		return JZ_FAILED;
	}

	return JZ_SUCCESS;
}

JZ_RESULT JZDevice::UnUse()
{
	if (!wglMakeCurrent(m_hDC, NULL))	// 设置为当前的渲染上下文环境
	{
		return JZ_FAILED;
	}
	return JZ_SUCCESS;
}

JZ_RESULT JZDevice::Release()
{
	bool bRet = wglMakeCurrent(m_hDC, NULL);// 当前渲染环境置空
	bRet = wglDeleteContext(m_hGLRC);// 删除渲染环境
	m_hGLRC = NULL;
	if (NULL != m_hDC)
	{
		m_hDC = NULL;
	}
	return JZ_SUCCESS;
}