#include "JZDevice.h"
#include <GL/glew.h>
#include <cassert>
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
	eRet = glGetError();
	assert(0 == eRet);
}

JZDevice::~JZDevice()
{
	bool bRet = wglMakeCurrent(m_hDC, NULL);// ��ǰ��Ⱦ�����ÿ�
	bRet = wglDeleteContext(m_hGLRC);// ɾ����Ⱦ����
	m_hGLRC = NULL;
	if (NULL != m_hDC)
	{
		m_hDC = NULL;
	}
}

JZResType JZDevice::GetResType()
{
	return JZ_RES_DEVICE;
}

JZ_RESULT JZDevice::CreateDevice(HWND hWnd, HDC hDC, HGLRC hShareRC)
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
	eRet = glGetError();
	assert(0 == eRet);

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