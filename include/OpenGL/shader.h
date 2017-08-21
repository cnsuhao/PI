#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h> // ����glew����ȡ���еı���OpenGLͷ�ļ�

class Shader
{
public:
	// ����ID
	GLuint Program;

public:
	// ��������ȡ��������ɫ��
	Shader(){}
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// ʹ�ó���
	void Use();
	// �ͷ���ɫ������
	void Delete();
};

#endif