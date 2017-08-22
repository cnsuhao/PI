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
	// ��ȡ��Դ����
	virtual JZResType GetResType() = 0;
	// ʹ����ɫ������
	virtual void Use() = 0;
	// shaderPath��ʾ������shader·�����ַ������飬iShaderNums��ʾ���ַ�����������˼���shader·�����ַ�������Ӧ�ð��ն�����ɫ����Ƭ����ɫ����������ɫ����˳������
	virtual JZ_RESULT CreateShaderProgram(const char** shaderPath, int iShaderNums) = 0;
	// ��ȡ��ɫ������ID
	virtual unsigned int GetProgramID() = 0;
	// �ͷ���ɫ������
	virtual void Delete() = 0;
};

#endif // !__IJZ_BASERENDER_H__

