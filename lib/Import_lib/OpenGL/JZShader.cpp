#include "JZShader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

JZShader::JZShader() 
{
	m_shaderProgram = 0;
	m_strVertex = NULL;		// 顶点着色器代码
	m_strFragment = NULL;	// 片段着色器代码
	m_strGeometry = NULL;	// 几何着色器代码
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

JZResType JZShader::GetResType()
{
	return JZ_RES_SHADER;
}

void JZShader::Use()
{
	assert(1 == glIsProgram(m_shaderProgram));
	glUseProgram(m_shaderProgram);
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

void JZShader::Delete()
{
	if (1 == glIsProgram(m_shaderProgram))
		glDeleteProgram(m_shaderProgram);
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
		m_strVertex = shaderCode.c_str();
		break;
	case JZ_SHADER_FRAGMENT:
		m_strFragment = shaderCode.c_str();
		break;
	case JZ_SHADER_GEOMETRY:
		m_strGeometry = shaderCode.c_str();
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

	if (NULL == m_strVertex || NULL == m_strFragment)
	{
		return JZ_FAILED;
	}

	// 编译顶点着色器
	if (NULL != m_strVertex)
	{
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &m_strVertex, NULL);
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
	
	// 编译片段着色器
	if (NULL != m_strFragment)
	{
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &m_strFragment, NULL);
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

	// 编译几何着色器
	if (NULL != m_strGeometry)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &m_strGeometry, NULL);
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

	// 创建和链接着色器程序
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	if (NULL != m_strGeometry)
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

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return JZ_SUCCESS;
}