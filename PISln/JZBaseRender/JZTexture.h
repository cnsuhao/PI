#ifndef __JZ_TEXTURE_H__
#define __JZ_TEXTURE_H__
#include <IJZBaseRender.h>

class JZTexture: public IJZTexture
{
public:
	JZTexture();
	~JZTexture();
	virtual JZResType GetResType();		// 获取资源类型
	virtual JZ_RESULT Create(int width, int height, JZImageBuf* pImageBuf = NULL);	// 创建纹理资源
	virtual JZ_RESULT Release();	// 释放纹理资源
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf);	// 往纹理中填充图像数据
	virtual unsigned int GetTexID();		// 获取该纹理ID
	virtual int GetTexWidth();	// 获取纹理宽度
	virtual int GetTexHeight();	// 获取纹理高度
private:
	unsigned int m_texID;
	int m_texWidth;
	int m_texHeight;
};



#endif // !__JZ_TEXTURE_H__

