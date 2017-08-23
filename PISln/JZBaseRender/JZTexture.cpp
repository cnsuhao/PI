#include <cassert>
#include "JZTexture.h"

JZTexture::JZTexture()
{
	m_texID = 0;
	m_texWidth = 0;
	m_texHeight = 0;
}

JZTexture::~JZTexture()
{
}

JZ_RESULT JZTexture::Create(GLsizei width, GLsizei height)
{
	if (0 == m_texID) // 需要创建纹理
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
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_texWidth, m_texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
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

GLuint JZTexture::GetTexID()
{
	return m_texID;
}

GLsizei JZTexture::GetTexWidth() // 获取纹理宽度
{
	return m_texWidth;
}
GLsizei JZTexture::GetTexHeight()// 获取纹理高度
{
	return m_texHeight;
}

JZTexManager::JZTexManager()
{
}

JZTexManager::~JZTexManager()
{
}

JZTexture* JZTexManager::CreateTexture()
{

}

JZ_RESULT JZTexManager::ReleaseTexture(JZTexture* pTexture)
{

}