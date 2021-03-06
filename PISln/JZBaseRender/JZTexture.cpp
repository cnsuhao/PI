#include <cassert>
#include "JZTexture.h"
#include <GL/glew.h> // 包含glew来获取所有的必须OpenGL头文件
#include <iostream>
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


JZTexture::JZTexture()
{
	m_texID = 0;
	m_texWidth = 0;
	m_texHeight = 0;
}

JZTexture::~JZTexture()
{
	Release();
}

JZ_RES_TYPE JZTexture::GetResType()
{
	return JZ_RES_TEX;
}

JZ_RESULT JZTexture::Create(JZImageBuf* pImageBuf)
{
	if (NULL == pImageBuf)
	{
		return JZ_FAILED;
	}

	if (0 == m_texID) // 需要创建纹理
	{
		m_texWidth = pImageBuf->width;
		m_texHeight = pImageBuf->height;
		GLint internalformat = GL_RGB;// 目前OpenGL内部像素格式仅支持RGB形式的
		GLenum outformat = 0;
		switch (pImageBuf->pixel_fmt)
		{
		case JZ_PIXFMT_BGR:
			outformat = GL_BGR;
			break;
		case JZ_PIXFMT_BGRA:
			outformat = GL_BGRA;
			break;
		case JZ_PIXFMT_RGB:
			outformat = GL_RGB;
			break;
		case JZ_PIXFMT_RGBA:
			outformat = GL_RGBA;
			break;
		default:
			outformat = GL_BGR;
			break;
		}
		
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		if (NULL != pImageBuf->color)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, m_texWidth, m_texHeight, 0, outformat, GL_UNSIGNED_BYTE, pImageBuf->color);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalformat, m_texWidth, m_texHeight, 0, outformat, GL_UNSIGNED_BYTE, 0);
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
		GLenum eRet = glGetError();
		assert(0 == eRet);
		return JZ_SUCCESS;
	}
	else // 该纹理已经创建了，不需要创建，直接返回即可
	{
		return JZ_SUCCESS;
	}
}

JZ_RESULT JZTexture::Release()
{
	if (0 != m_texID)
	{
		glDeleteTextures(1, &m_texID);
		m_texID = 0;
		m_texWidth = 0;
		m_texHeight = 0;
		GLenum eRet = glGetError();
		assert(0 == eRet);
	}
	return JZ_SUCCESS;
}

JZ_RESULT JZTexture::FillImage(JZImageBuf* pImageBuf)
{
	if (JZ_PIXFMT_RGB == pImageBuf->pixel_fmt && m_texWidth == pImageBuf->width && m_texHeight == pImageBuf->height)
	{
		GLint internalformat = GL_RGB;// 目前OpenGL内部像素格式仅支持RGB形式的
		GLenum outformat = 0;
		switch (pImageBuf->pixel_fmt)
		{
		case JZ_PIXFMT_BGR:
			outformat = GL_BGR;
			break;
		case JZ_PIXFMT_BGRA:
			outformat = GL_BGRA;
			break;
		case JZ_PIXFMT_RGB:
			outformat = GL_RGB;
			break;
		case JZ_PIXFMT_RGBA:
			outformat = GL_RGBA;
			break;
		default:
			outformat = GL_BGR;
			break;
		}

		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalformat, m_texWidth, m_texHeight, 0, outformat, GL_UNSIGNED_BYTE, pImageBuf->color);
		glBindTexture(GL_TEXTURE_2D, 0);
		GLenum eRet = glGetError();
		assert(0 == eRet);
		return JZ_SUCCESS;
	}
	else
	{
		return JZ_FAILED;
	}
}

unsigned int JZTexture::GetTexID()
{
	return m_texID;
}

int JZTexture::GetTexWidth() // 获取纹理宽度
{
	return m_texWidth;
}
int JZTexture::GetTexHeight()// 获取纹理高度
{
	return m_texHeight;
}
