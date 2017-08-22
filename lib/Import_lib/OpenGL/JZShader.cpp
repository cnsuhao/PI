#include "JZShader.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>

JZShader::JZShader() 
{
	m_shaderProgram = 0;
	m_strVertex = NULL;		// ������ɫ������
	m_strFragment = NULL;	// Ƭ����ɫ������
	m_strGeometry = NULL;	// ������ɫ������
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

JZResType JZShader::GetResType()
{
	return JZ_RES_SHADER;
}

void JZShader::Use()
{
	assert(1 == glIsProgram(m_shaderProgram));
	glUseProgram(m_shaderProgram);
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

	// ���붥����ɫ��
	if (NULL != m_strVertex)
	{
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &m_strVertex, NULL);
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
	
	// ����Ƭ����ɫ��
	if (NULL != m_strFragment)
	{
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &m_strFragment, NULL);
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

	// ���뼸����ɫ��
	if (NULL != m_strGeometry)
	{
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &m_strGeometry, NULL);
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

	// ������������ɫ������
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram, vertex);
	glAttachShader(m_shaderProgram, fragment);
	if (NULL != m_strGeometry)
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

	// ɾ����ɫ���������Ѿ����ӵ����ǵĳ������ˣ��Ѿ�������Ҫ��
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	glDeleteShader(geometry);

	return JZ_SUCCESS;
}