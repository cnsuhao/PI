#ifndef __JZ_SHADER_H__
#define __JZ_SHADER_H__
#include <IJZBaseRender.h>
#include <GL/glew.h> // 包含glew来获取所有的必须OpenGL头文件
#include <string>

enum JZShaderType
{
	JZ_SHADER_UNKNOW,
	JZ_SHADER_VERTEX ,
	JZ_SHADER_FRAGMENT,
	JZ_SHADER_GEOMETRY,
};

class JZShader :public IJZShader
{

public:
	// 构造器读取并构建着色器
	JZShader();
	JZShader(const char** shaderPath, int iShaderNums);
	// 获取资源类型
	JZResType GetResType();
	// 使用着色器程序
	void Use();
	// shaderPath表示包含了shader路径的字符串数组，iShaderNums表示该字符串数组包含了几个shader路径，字符串数组应该按照顶点着色器、片段着色器、几何着色器的顺序排列
	JZ_RESULT CreateShaderProgram(const char** shaderPath, int iShaderNums);
	// 获取着色器程序ID
	unsigned int GetProgramID();
	// 释放着色器程序
	void Delete();

private:
	// 根据shader路径，读取shader代码，并设置给对应的成员变量
	void _SetShaderCode(const char* shaderPath, JZShaderType shaderType);
	// 编译shader
	JZ_RESULT _CompileShaderCode();

public:
	// 程序ID
	GLuint m_shaderProgram;
	std::string m_strVertex;	// 顶点着色器代码
	std::string m_strFragment;	// 片段着色器代码
	std::string m_strGeometry;	// 几何着色器代码
};

#endif // !__JZ_SHADER_H__