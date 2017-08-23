#ifndef __JZ_TEXTURE_H__
#define __JZ_TEXTURE_H__
#include <IJZBaseRender.h>
#include <GL/glew.h> // 包含glew来获取所有的必须OpenGL头文件
#include <vector>
class JZTexture
{
public:
	JZTexture();
	~JZTexture();
	JZ_RESULT Create(GLsizei width, GLsizei height);	// 创建纹理资源
	JZ_RESULT Release();	// 释放纹理资源
	JZ_RESULT FillImage(JZImageBuf* pImageBuf);	// 往纹理中填充图像数据
	GLuint GetTexID();		// 获取该纹理ID
	GLsizei GetTexWidth();	// 获取纹理宽度
	GLsizei GetTexHeight();	// 获取纹理高度
private:
	GLuint m_texID;
	GLsizei m_texWidth;
	GLsizei m_texHeight;
};

class JZTexManager
{
public:
	JZTexManager();
	~JZTexManager();
	JZTexture* CreateTexture();
	JZ_RESULT ReleaseTexture(JZTexture* pTexture);

private:
	std::vector<GLuint> m_vecTexID; // 已经创建的并且没有被释放的纹理资源的ID
};


#endif // !__JZ_TEXTURE_H__

