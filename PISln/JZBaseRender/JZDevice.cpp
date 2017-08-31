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
	if (!(pixelformat = ChoosePixelFormat(m_hDC, &pfd)))	// ��ȡ��ƥ������ظ�ʽ
	{
		return ;
	}
	if (!SetPixelFormat(m_hDC, pixelformat, &pfd))	// �����������ظ�ʽ�������wgl��������
	{
		return ;
	}
	if (!(m_hGLRC = wglCreateContext(m_hDC)))	// ����OpenGL��Ⱦ����
	{
		return ;
	}
	if (!wglMakeCurrent(m_hDC, m_hGLRC))	// ����Ϊ��ǰ����Ⱦ�����Ļ���
	{
		return ;
	}

	glewExperimental = GL_TRUE;
	glewInit();

	RECT rect; //����������л�ͼ  
	GetClientRect(hWnd, &rect);
	//��������ռ���������ڴ�С 
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
	if (!(pixelformat = ChoosePixelFormat(m_hDC, &pfd)))	// ��ȡ��ƥ������ظ�ʽ
	{
		return JZ_FAILED;
	}
	if (!SetPixelFormat(m_hDC, pixelformat, &pfd))	// �����������ظ�ʽ�������wgl��������
	{
		return JZ_FAILED;
	}
	if (!(m_hGLRC = wglCreateContext(m_hDC)))	// ����OpenGL��Ⱦ����
	{
		return JZ_FAILED;
	}
	if (!wglMakeCurrent(m_hDC, m_hGLRC))	// ����Ϊ��ǰ����Ⱦ�����Ļ���
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

	if (!wglMakeCurrent(m_hDC, m_hGLRC))	// ����Ϊ��ǰ����Ⱦ�����Ļ���
	{
		return JZ_FAILED;
	}

	return JZ_SUCCESS;
}

JZ_RESULT JZDevice::UnUse()
{
	if (!wglMakeCurrent(m_hDC, NULL))	// ����Ϊ��ǰ����Ⱦ�����Ļ���
	{
		return JZ_FAILED;
	}
	return JZ_SUCCESS;
}

JZ_RESULT JZDevice::Release()
{
	bool bRet = wglMakeCurrent(m_hDC, NULL);// ��ǰ��Ⱦ�����ÿ�
	bRet = wglDeleteContext(m_hGLRC);// ɾ����Ⱦ����
	m_hGLRC = NULL;
	if (NULL != m_hDC)
	{
		m_hDC = NULL;
	}
	return JZ_SUCCESS;
}