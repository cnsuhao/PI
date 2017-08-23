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
	m_strVertex = "";		// ������ɫ������
	m_strFragment = "";		// Ƭ����ɫ������
	m_strGeometry = "";		// ������ɫ������
}

JZShader::JZShader(const char** shaderPath, int iShaderNums)
{
	// ����������
	if (NULL == shaderPath || NULL == *shaderPath || iShaderNums <= 1)
	{
		return;
	}

	// ���ַ��������з����������ɫ��·��
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

	// ����shader����
	_SetShaderCode(vertexPath, JZ_SHADER_VERTEX);
	_SetShaderCode(fragmentPath, JZ_SHADER_FRAGMENT);
	_SetShaderCode(geometryPath, JZ_SHADER_GEOMETRY);

	// ������ɫ��
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

// ���ʹ����ɫ������
void JZShader::UnUse()
{
	glUseProgram(0);
	GLenum eRet = glGetError();
	assert(0 == eRet);
}

// shaderPath��ʾ������shader·�����ַ������飬iShaderNums��ʾ���ַ�����������˼���shader·�����ַ�������Ӧ�ð��ն�����ɫ����Ƭ����ɫ����������ɫ��
JZ_RESULT JZShader::CreateShaderProgram(const char** shaderPath, int iShaderNums)
{
	// ����������
	if (NULL == shaderPath || NULL == *shaderPath || iShaderNums <=1)
	{
		return JZ_FAILED;
	}

	// ���ַ��������з����������ɫ��·��
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
	
	// ����shader����
	_SetShaderCode(vertexPath, JZ_SHADER_VERTEX);
	_SetShaderCode(fragmentPath, JZ_SHADER_FRAGMENT);
	_SetShaderCode(geometryPath, JZ_SHADER_GEOMETRY);

	// ������ɫ��
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

	// 1. ���ļ�·���л�ȡ����/Ƭ����ɫ��
	std::string shaderCode;
	std::ifstream shaderFile;
	std::stringstream shaderStream;

	// ��֤ifstream��������׳��쳣��
	shaderFile.exceptions(std::ifstream::badbit);
	try
	{
		shaderFile.open(shaderPath);// ���ļ�
		shaderStream << shaderFile.rdbuf();// ��ȡ�ļ��Ļ������ݵ�����
		shaderFile.close();// �ر��ļ�
		shaderCode = shaderStream.str();// ת������GLchar����
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

	// ���붥����ɫ��
	if (!m_strVertex.empty())
	{
		vertex = glCreateShader(GL_VERTEX_SHADER);
		const char* strVertex = m_strVertex.c_str();
		glShaderSource(vertex, 1, &strVertex, NULL);
		glCompileShader(vertex);
		// ��ӡ�����������еĻ���
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			return JZ_FAILED;
		}
	}
	eRet = glGetError(); assert(0 == eRet);
	// ����Ƭ����ɫ��
	if (!m_strFragment.empty())
	{
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		const char* strFragment = m_strFragment.c_str();
		glShaderSource(fragment, 1, &strFragment, NULL);
		glCompileShader(fragment);
		// ��ӡ�����������еĻ���
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
			return JZ_FAILED;
		}
	}
	eRet = glGetError(); assert(0 == eRet);
	// ���뼸����ɫ��
	if (!m_strGeometry.empty())
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		const char* strGeometry = m_strGeometry.c_str();
		glShaderSource(geometry, 1, &strGeometry, NULL);
		glCompileShader(geometry);
		// ��ӡ�����������еĻ���
		glGetShaderiv(geometry, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(geometry, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
			return JZ_FAILED;
		}
	}
	eRet = glGetError(); assert(0 == eRet);
	// ������������ɫ������
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	if (!m_strGeometry.empty())
	{
		glAttachShader(m_shaderProgram, geometry);
	}
	glLinkProgram(m_shaderProgram);
	// ��ӡ���Ӵ�������еĻ���
	glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	eRet = glGetError(); assert(0 == eRet);
	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (!m_strGeometry.empty())
	{
		glDeleteShader(geometry);
	}
	eRet = glGetError(); assert(0 == eRet);
	return JZ_SUCCESS;
}