#include <cassert>
#include "JZTexture.h"
#include <GL/glew.h> // ����glew����ȡ���еı���OpenGLͷ�ļ�

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

JZResType JZTexture::GetResType()
{
	return JZ_RES_TEX;
}

JZ_RESULT JZTexture::Create(int width, int height, JZImageBuf* pImageBuf)
{
	if (0 == m_texID) // ��Ҫ��������
	{
		m_texWidth = width;
		m_texHeight = height;
		glGenTextures(1, &m_texID);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		if (NULL != pImageBuf && NULL != pImageBuf->color)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageBuf->color);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
		}
		
		glBindTexture(GL_TEXTURE_2D, 0);
		GLenum eRet = glGetError();
		assert(0 == eRet);
		return JZ_SUCCESS;
	}
	else // �������Ѿ������ˣ�����Ҫ������ֱ�ӷ��ؼ���
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
	if (JZ_PIXFMT_RGB == pImageBuf->pixel_fmt)
	{
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_2D, m_texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, pImageBuf->color);
		glBindTexture(GL_TEXTURE_2D, 0);
		GLenum eRet = glGetError();
		assert(0 == eRet);
		return JZ_SUCCESS;
	}
}

unsigned int JZTexture::GetTexID()
{
	return m_texID;
}

int JZTexture::GetTexWidth() // ��ȡ������
{
	return m_texWidth;
}
int JZTexture::GetTexHeight()// ��ȡ����߶�
{
	return m_texHeight;
}
