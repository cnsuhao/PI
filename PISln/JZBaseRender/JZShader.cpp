#include "JZShader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

#ifdef _DEBUG
#	ifdef _WIN64
#		pragma comment(lib, "glew64d.lib")
#		pragma comment(lib, "SOIL64s_d.lib")
#	else
#		pragma comment(lib, "glew32d.lib")
#		pragma comment(lib, "SOIL32s_d.lib")
#	endif
#else
#	ifdef _WIN64
#		pragma comment(lib, "glew64.lib")
#		pragma comment(lib, "SOIL64s.lib")
#	else
#		pragma comment(lib, "glew32.lib")
#		pragma comment(lib, "SOIL32s.lib")
#	endif
#endif // _DEBUG
#pragma comment(lib, "opengl32.lib")

JZShader::JZShader() 
{
	m_shaderProgram = 0;
	m_strVertex = "";		// 顶点着色器代码
	m_strFragment = "";		// 片段着色器代码
	m_strGeometry = "";		// 几何着色器代码
}

JZShader::JZShader(const char** shaderPath, int iShaderNums)
{
	// 输入参数检查
	if (NULL == shaderPath || NULL == *shaderPath || iShaderNums <= 1)
	{
		return;
	}

	// 从字符串数组中分离出各个着色器路径
	const char* vertexPath = NULL;
	const char* fragmentPath = NULL;
	const char* geometryPath = NULL;
	if (iShaderNums >= 2)
	{
		vertexPath = shaderPath[0];
		fragmentPath = shaderPath[1];
		if (iShaderNums >= 3)
		{
			geometryPath = shaderPath[2];
		}
	}

	// 设置shader代码
	_SetShaderCode(vertexPath, JZ_SHADER_VERTEX);
	_SetShaderCode(fragmentPath, JZ_SHADER_FRAGMENT);
	_SetShaderCode(geometryPath, JZ_SHADER_GEOMETRY);

	// 编译着色器
	_CompileShaderCode();
}

JZShader::~JZShader()
{
	Release();
}

JZResType JZShader::GetResType()
{
	return JZ_RES_SHADER;
}

void JZShader::Use()
{
	assert(1 == glIsProgram(m_shaderProgram));
	glUseProgram(m_shaderProgram);
}

// 解除使用着色器程序
void JZShader::UnUse()
{
	glUseProgram(0);
	GLenum eRet = glGetError();
	assert(0 == eRet);
}

// shaderPath表示包含了shader路径的字符串数组，iShaderNums表示该字符串数组包含了几个shader路径，字符串数组应该按照顶点着色器、片段着色器、像素着色器
JZ_RESULT JZShader::CreateShaderProgram(const char** shaderPath, int iShaderNums)
{
	// 输入参数检查
	if (NULL == shaderPath || NULL == *shaderPath || iShaderNums <=1)
	{
		return JZ_FAILED;
	}

	// 从字符串数组中分离出各个着色器路径
	const char* vertexPath = NULL;
	const char* fragmentPath = NULL;
	const char* geometryPath = NULL;
	if (iShaderNums >= 2)
	{
		vertexPath = shaderPath[0]; 
		fragmentPath = shaderPath[1];
		if (iShaderNums >= 3)
		{
			geometryPath = shaderPath[2];
		}
	}
	
	// 设置shader代码
	_SetShaderCode(vertexPath, JZ_SHADER_VERTEX);
	_SetShaderCode(fragmentPath, JZ_SHADER_FRAGMENT);
	_SetShaderCode(geometryPath, JZ_SHADER_GEOMETRY);

	// 编译着色器
	return _CompileShaderCode();
}

unsigned int JZShader::GetProgramID()
{
	return m_shaderProgram;
}

void JZShader::Release()
{
	if (1 == glIsProgram(m_shaderProgram))
	{
		glDeleteProgram(m_shaderProgram);
		m_shaderProgram = 0;
	}
		
}

void JZShader::_SetShaderCode(const char* shaderPath, JZShaderType shaderType)
{
	if (NULL == shaderPath || JZ_SHADER_UNKNOW == shaderType)
	{
		return ;
	}

	// 1. 从文件路径中获取顶点/片段着色器
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	// 保证ifstream对象可以抛出异常：
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);// 打开文件
		shaderStream << shaderFile.rdbuf();// 读取文件的缓冲内容到流中
		shaderFile.close();// 关闭文件
		shaderCode = shaderStream.str();// 转换流至GLchar数组
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	switch (shaderType)
	{
	case JZ_SHADER_VERTEX:
		m_strVertex = shaderCode;
		break;
	case JZ_SHADER_FRAGMENT:
		m_strFragment = shaderCode;
		break;
	case JZ_SHADER_GEOMETRY:
		m_strGeometry = shaderCode;
		break;
	default:
		break;
	}
}

JZ_RESULT JZShader::_CompileShaderCode()
{
	GLuint vertex, fragment, geometry;
	GLint success;
	GLchar infoLog[512];
	GLenum eRet;
	if (m_strVertex.empty() || m_strFragment.empty())
	{
		return JZ_FAILED;
	}

	// 编译顶点着色器
	if (!m_strVertex.empty())
	{
		vertex = glCreateShader(GL_VERTEX_SHADER);
		const char* strVertex = m_strVertex.c_str();
		glShaderSource(vertex, 1, &strVertex, NULL);
		glCompileShader(vertex);
		// 打印编译错误（如果有的话）
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return JZ_FAILED;
		}
	}
	eRet = glGetError(); assert(0 == eRet);
	// 编译片段着色器
	if (!m_strFragment.empty())
	{
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		const char* strFragment = m_strFragment.c_str();
		glShaderSource(fragment, 1, &strFragment, NULL);
		glCompileShader(fragment);
		// 打印编译错误（如果有的话）
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			return JZ_FAILED;
		}
	}
	eRet = glGetError(); assert(0 == eRet);
	// 编译几何着色器
	if (!m_strGeometry.empty())
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		const char* strGeometry = m_strGeometry.c_str();
		glShaderSource(geometry, 1, &strGeometry, NULL);
		glCompileShader(geometry);
		// 打印编译错误（如果有的话）
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			return JZ_FAILED;
		}
	}
	eRet = glGetError(); assert(0 == eRet);
	// 创建和链接着色器程序
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	if (!m_strGeometry.empty())
	{
		glAttachShader(m_shaderProgram, geometry);
	}
	glLinkProgram(m_shaderProgram);
	// 打印连接错误（如果有的话）
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	eRet = glGetError(); assert(0 == eRet);
	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!m_strGeometry.empty())
	{
		glDeleteShader(geometry);
	}
	eRet = glGetError(); assert(0 == eRet);
	return JZ_SUCCESS;
}