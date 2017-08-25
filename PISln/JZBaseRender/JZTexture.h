#ifndef __JZ_TEXTURE_H__
#define __JZ_TEXTURE_H__
#include <IJZBaseRender.h>

class JZTexture: public IJZTexture
{
public:
	// 构造函数
	JZTexture();
	// 析构函数
	~JZTexture();
	// 获取资源类型
	virtual JZResType GetResType();		
	// 创建纹理资源，目前仅支持RGB图像
	virtual JZ_RESULT Create(JZImageBuf* pImageBuf);	
	// 释放纹理资源
	virtual JZ_RESULT Release();	
	// 往纹理中填充图像数据，仅支持RGB图像，并且只支持和已经创建的纹理宽高一样的情况，该接口慎用
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf);	
	// 获取该纹理ID
	virtual unsigned int GetTexID();		
	// 获取纹理宽度
	virtual int GetTexWidth();	
	// 获取纹理高度
	virtual int GetTexHeight();
private:
	unsigned int m_texID;		// 纹理ID
	int m_texWidth;				// 纹理宽度
	int m_texHeight;			// 纹理高度
};



#endif // !__JZ_TEXTURE_H__

