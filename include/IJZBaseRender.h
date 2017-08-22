#ifndef __IJZ_BASERENDER_H__
#define __IJZ_BASERENDER_H__
#include <JZCommonImageDef.h>

enum JZResType
{
	JZ_RES_SHADER = 0,
	JZ_RES_TEX,
};
class IJZBaseRes
{
public:
	IJZBaseRes() {}
	~IJZBaseRes() {}
	virtual JZResType GetResType() = 0;
};

class IJZBaseRender
{
public:
	IJZBaseRender() {}
	~IJZBaseRender() {}
	virtual JZ_RESULT InitOpenGL() = 0;

private:

};

class IJZShader :public IJZBaseRes
{
public:
	// 获取资源类型
	virtual JZResType GetResType() = 0;
	// 使用着色器程序
	virtual void Use() = 0;
	// shaderPath表示包含了shader路径的字符串数组，iShaderNums表示该字符串数组包含了几个shader路径，字符串数组应该按照顶点着色器、片段着色器、几何着色器的顺序排列
	virtual JZ_RESULT CreateShaderProgram(const char** shaderPath, int iShaderNums) = 0;
	// 获取着色器程序ID
	virtual unsigned int GetProgramID() = 0;
	// 释放着色器程序
	virtual void Delete() = 0;
};

#endif // !__IJZ_BASERENDER_H__

