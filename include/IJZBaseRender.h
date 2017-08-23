#ifndef __IJZ_BASERENDER_H__
#define __IJZ_BASERENDER_H__
#include <JZCommonImageDef.h>
#include <Windows.h>
enum JZResType
{
	JZ_RES_DEVICE,
	JZ_RES_SHADER,
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

class IJZDevice : public IJZBaseRes
{
public:
	virtual JZResType GetResType() = 0;
	virtual JZ_RESULT CreateDevice(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL) = 0;
	virtual JZ_RESULT Use() = 0;
	virtual JZ_RESULT UnUse() = 0;
	virtual JZ_RESULT Release() = 0;
};

class IJZShader: public IJZBaseRes
{
public:
	// ��ȡ��Դ����
	virtual JZResType GetResType() = 0;
	// ʹ����ɫ������
	virtual void Use() = 0;
	// ���ʹ����ɫ������
	virtual void UnUse() = 0;
	// shaderPath��ʾ������shader·�����ַ������飬iShaderNums��ʾ���ַ�����������˼���shader·�����ַ�������Ӧ�ð��ն�����ɫ����Ƭ����ɫ����������ɫ����˳������
	virtual JZ_RESULT CreateShaderProgram(const char** shaderPath, int iShaderNums) = 0;
	// ��ȡ��ɫ������ID
	virtual unsigned int GetProgramID() = 0;
	// �ͷ���ɫ������
	virtual void Release() = 0;
};

class IJZTexture: public IJZBaseRes
{
public:
	virtual JZResType GetResType() = 0;
	virtual JZ_RESULT Create(int width, int height, JZImageBuf* pImageBuf = NULL) = 0;	// ����������Դ��Ŀǰ��֧��RBG��buffer
	virtual JZ_RESULT Release() = 0;	// �ͷ�������Դ
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf) = 0;	// �����������ͼ�����ݣ�Ŀǰ��֧��RBG��buffer
	virtual unsigned int GetTexID() = 0;		// ��ȡ������ID
	virtual int GetTexWidth() = 0;	// ��ȡ������
	virtual int GetTexHeight() = 0;	// ��ȡ����߶�
};

class IJZScene: public IJZBaseRender
{
public:
	virtual void SetHWnd(HWND cwnd) = 0;
	virtual JZ_RESULT InitOpenGL() = 0;
	virtual void PrepareData() = 0;
	virtual void PushDataToGPU() = 0;
	virtual void RenderScene() = 0;
};

#endif // !__IJZ_BASERENDER_H__

