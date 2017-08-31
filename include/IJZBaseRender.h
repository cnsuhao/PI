#ifndef __IJZ_BASERENDER_H__
#define __IJZ_BASERENDER_H__
#include <JZCommonImageDef.h>
#include <vector>
#include <glm/glm.hpp>
#include <Windows.h>

enum JZ_RES_TYPE
{
	JZ_RES_UNKNOW,
	JZ_RES_DEVICE,			// OpenGL�豸��Դ
	JZ_RES_SHADER,			// OpenGL shader��Դ
	JZ_RES_TEX,				// OpenGL ������Դ
	JZ_RES_MESH,			// OpenGL������Դ
};

enum JZ_DRAW_UNIT
{
	JZ_DRAW_POINTS,			// ��ӦOpenGL��GL_POINTS
	JZ_DRAW_LINES,			// ��ӦOpenGL��GL_LINES
	JZ_DRAW_TRIANGLES,		// ��ӦOpenGL��GL_TRIANGLES
};

enum JZ_DRAW_METHOD
{
	JZ_DRAW_ARRAYS,			// ��ӦOpenGL��glDrawArrays()����
	JZ_DRAW_ELEMENTS,		// ��ӦOpenGL��glDrawElements()����
};

struct Vertex
{
	glm::vec3 position;		// ��������
	glm::vec3 color;		// ������ɫ
	glm::vec2 texture;		// ������������
};

// ��Դ����
class IJZBaseRes
{
public:
	// ��ȡ��Դ����
	virtual JZ_RES_TYPE GetResType() = 0;	
};

class IJZBaseRender
{
public:
	IJZBaseRender() {}
	~IJZBaseRender() {}
};

// OpenGL�豸��Դ
class IJZDevice : public IJZBaseRes
{
public:
	// ��ȡ��Դ����
	virtual JZ_RES_TYPE GetResType() = 0;
	// �����豸��Դ
	virtual JZ_RESULT Create(HWND hWnd, HDC hDC = NULL, HGLRC hShareRC = NULL) = 0;
	// �ͷ��豸��Դ
	virtual JZ_RESULT Release() = 0;
	// ʹ��(��)�豸
	virtual JZ_RESULT Use() = 0;
	// �����
	virtual JZ_RESULT UnUse() = 0;
};

// OpenGL��shader��Դ
class IJZShader: public IJZBaseRes
{
public:
	// ��ȡ��Դ����
	virtual JZ_RES_TYPE GetResType() = 0;
	// ʹ����ɫ������
	virtual void Use() = 0;
	// ���ʹ����ɫ������
	virtual void UnUse() = 0;
	// shaderName��ʾ������shader·�����ַ������飬iShaderNums��ʾ���ַ�����������˼���shader·�����ַ�������Ӧ�ð��ն�����ɫ����Ƭ����ɫ����������ɫ����˳������
	virtual JZ_RESULT Create(const char** shaderName, int iShaderNums) = 0;
	// ��ȡ��ɫ������ID
	virtual unsigned int GetProgramID() = 0;
	// �ͷ���ɫ������
	virtual void Release() = 0;
};

// OpenGL������Դ
class IJZTexture: public IJZBaseRes
{
public:
	// ��ȡ��Դ����
	virtual JZ_RES_TYPE GetResType() = 0;
	// ����������Դ��Ŀǰ��֧��RGBͼ��
	virtual JZ_RESULT Create(JZImageBuf* pImageBuf) = 0;	
	// �ͷ�������Դ
	virtual JZ_RESULT Release() = 0;
	// �����������ͼ�����ݣ�Ŀǰֻ֧�ֺ��Ѿ�������������һ����������ýӿ�����
	virtual JZ_RESULT FillImage(JZImageBuf* pImageBuf) = 0;	
	// ��ȡ������ID
	virtual unsigned int GetTexID() = 0;		
	// ��ȡ������
	virtual int GetTexWidth() = 0;
	// ��ȡ����߶�
	virtual int GetTexHeight() = 0;	
};

// OpenGL������Դ
class IJZMesh: public IJZBaseRes
{
public:
	// ��ȡ��Դ����
	virtual JZ_RES_TYPE GetResType() = 0;
	// ����������Դ
	virtual JZ_RESULT Create(std::vector<Vertex> vertexSet, std::vector<unsigned int> indexSet, JZ_DRAW_UNIT drawUnit, JZ_DRAW_METHOD drawMethod) = 0;
	// ����������ʾͼ���������Դ
	virtual JZ_RESULT CreateQuadMesh() = 0;
	// ���ݻ�������Ŀ�߱Ⱥ������߱ȴ���������Դ
	virtual JZ_RESULT CreateQuadMesh(float rectAspect, float texAspect) = 0;
	// �ͷ�������Դ
	virtual JZ_RESULT Release() = 0;
	// ��������
	virtual JZ_RESULT Draw() = 0;
};

//
class IJZSceneRender: public IJZBaseRender
{
public:
	// ����OpenGL�豸
	virtual JZ_RESULT SetDevice(HWND hWnd) = 0;
	// ����shader��Դ
	virtual void SetShader() = 0;
	// ����Ҫ��Ⱦ�����ͼ�� ��֧��RGB
	virtual void SetLeftImage(JZImageBuf* pImageBuf = NULL) = 0;
	// ����Ҫ��Ⱦ���Ҳ�ͼ�� ��֧��RGB
	virtual void SetRightImage(JZImageBuf* pImageBuf = NULL) = 0;
	// ���ñ�����ɫ
	virtual void SetGroundColor(glm::vec4 color = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)) = 0;
	// ��ʼ����Ⱦ��Ҫ�Ļ�������
	virtual void init(HWND hWnd) = 0;
	// ��Ⱦ����ͼ��
	virtual void RenderScene() = 0;
	// �ͷ��ڴ�
	virtual void Release() = 0;
};

typedef JZ_RESULT(*DefGetSceneInterface)(IJZSceneRender** ppScene);
typedef JZ_RESULT(*DefReleaseSceneInterface)(IJZSceneRender*& pScene);

struct JZBaseRenderAPI
{
	DefGetSceneInterface pfnGetSceneInterface;
	DefReleaseSceneInterface pfnReleaseSceneInterface;
};

#endif // !__IJZ_BASERENDER_H__

