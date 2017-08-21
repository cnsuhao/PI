#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h> // 包含glew来获取所有的必须OpenGL头文件

class Shader
{
public:
	// 程序ID
	GLuint Program;

public:
	// 构造器读取并构建着色器
	Shader(){}
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// 使用程序
	void Use();
	// 释放着色器程序
	void Delete();
};

#endif