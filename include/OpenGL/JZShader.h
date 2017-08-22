#ifndef __JZ_SHADER_H__
#define __JZ_SHADER_H__
#include <IJZBaseRender.h>
#include <GL/glew.h> // ����glew����ȡ���еı���OpenGLͷ�ļ�
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
	// ��������ȡ��������ɫ��
	JZShader();
	JZShader(const char** shaderPath, int iShaderNums);
	// ��ȡ��Դ����
	JZResType GetResType();
	// ʹ����ɫ������
	void Use();
	// shaderPath��ʾ������shader·�����ַ������飬iShaderNums��ʾ���ַ�����������˼���shader·�����ַ�������Ӧ�ð��ն�����ɫ����Ƭ����ɫ����������ɫ����˳������
	JZ_RESULT CreateShaderProgram(const char** shaderPath, int iShaderNums);
	// ��ȡ��ɫ������ID
	unsigned int GetProgramID();
	// �ͷ���ɫ������
	void Delete();

private:
	// ����shader·������ȡshader���룬�����ø���Ӧ�ĳ�Ա����
	void _SetShaderCode(const char* shaderPath, JZShaderType shaderType);
	// ����shader
	JZ_RESULT _CompileShaderCode();

public:
	// ����ID
	GLuint m_shaderProgram;
	std::string m_strVertex;	// ������ɫ������
	std::string m_strFragment;	// Ƭ����ɫ������
	std::string m_strGeometry;	// ������ɫ������
};

#endif // !__JZ_SHADER_H__