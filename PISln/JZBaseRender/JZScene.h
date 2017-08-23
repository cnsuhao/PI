#ifndef SCENE_H
#define SCENE_H

/////////////////ͷ�ļ�///////////////////////
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL/SOIL.h>
#include "IJZBaseRender.h"
#include "camera.h"
#include <Windows.h>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texture;
};

class JZScene: public IJZScene
{
public:
	/////////////////////////////��1��OpenGL��ʼ��ʱ�õ���һЩ����////////////////////////
	Camera camera;					// �������
	HWND m_hwnd;					// MFC�пؼ��ľ�������ú���GetDlgItem()��ȡ
	glm::vec4 groundColor;			// ��ʾ���򱳾���ɫRGBA
	glm::vec3 pointColor;			// ���Ƶ����ɫRGB
	std::vector<Vertex> pointArray; // ������ڻ��Ƶ������

	/////////////////////////////��2�����Ʋ����õ��ı���
	const char** m_shaderPath;		// shader·������
	int m_shaderNums;				// shader·�������а����˼�����ɫ��,���ն�����ɫ����Ƭ����ɫ����������ɫ����˳������
	IJZShader* m_pShader;			// OpenGL�е�shader��Դ
	IJZTexture* m_pTexture;			// OpenGL�е�������Դ
	IJZDevice* m_pDevice;			// OpenGL�豸��Դ

	GLuint m_VAO, m_VBO, m_EBO;		// OpenGL��VAO��VBO
	glm::mat4 modelMatrix;			// ģ�;���
	glm::mat4 viewMatrix;			// �۲����
	glm::mat4 projectionMatrix;		// ͶӰ����

	//////////////////////////// ��3����Ӧ���Ͱ����õ��ı���///////////////////////////////
	glm::vec2 downPoint;			// ����������ʱ��������ڵ�����
	glm::vec2 upPoint;				// �ɿ�������ʱ��������ڵ�����
	bool keys[1024];				// ���̰�����״̬��true��ʾ���£�false��ʾ�ɿ�
	float lastFrameTime;			// ��һ֡��ʱ��
	float deltaTime;				// ��֡����һ֡��ʱ���

	glm::vec2 lastFramePoint;		// ��һ֡���ָ�����ڵ�����
	bool firstCursor;				// �Ƿ�������һ���϶�ʱ��λ��
	bool isCSModel;					// �Ƿ���CSģʽ

public:
	JZScene();	// ���캯��
	~JZScene();	// ��������

	/////////////////////////////1.��ʼ�����ֵĺ���///////////////////////////////////////
public:
	void SetHWnd(HWND cwnd);											// ���ô��ڻ�ؼ����
	void SetGroundColor(glm::vec4 color);								// ���ñ�����ɫ
	void SetPointColor(glm::vec3 color);								// ���û��Ƶ���ɫ
	void InitMatrix();													// ��ʼ���任����
	void Reset();														// ��λ�ӽ�
	void PrepareData();													// ׼����������
	void PushDataToGPU();												// �����ݴ��ڴ洫���Դ�
	JZ_RESULT InitOpenGL();												// ��ʼ��OpenGL������

	/////////////////////////////2.���Ʋ��ֵĺ���////////////////////////////////////////
	void AddPoint(Vertex point);										// ����Ƶ���������ӻ��Ƶ�
	void DeletePoint(glm::vec3 point);									// �ӻ��Ƶ�������ɾ����
	void RenderScene();													// ��Ⱦ����ͼ��

	/////////////////////////////3.��Ӧ���Ͱ������ֵĺ���///////////////////////////////////
public:
	void SetDownPoint(glm::vec2 point);									// ��������������ʱ��������ڵ�����
	void SetUpPoint(glm::vec2 point);									// �����������ɿ�ʱ��������ڵ�����
	void SetKeyState(UINT nChar, bool state, bool resetAll = false);	// ���ü��̰�����״̬
	void SwitchViewModel();												// ��CSģʽ�ͷ�CSģʽ֮���л�
	void MouseControlView(POINT currentPoint);							// ͨ���������ӽǣ��������currentPoint������ڿͻ���������λ��
	void MouseWheelControlProjection(short zDelta, POINT point);		// ���ָı�ͶӰ����

private:
	void _UpdateViewAndProjectionMatrix();								// ���±任����
	bool _IsCursorInRect();												// �ж����ָ���Ƿ���ͼ�λ���������
	void _KeyControlCameraAndModel();									// ��סW��A��S��Dʱ������ӽǽ�����Ӧ�ı�
	void _MouseControlModel();											// ͨ�����������ģ����ת

	////////////////////////////4.�ͷ��ڴ沿�ֵĺ���///////////////////////////////////
public:
	void DestroyOpenGL();												// �ͷ�OpenGL���

};

#endif