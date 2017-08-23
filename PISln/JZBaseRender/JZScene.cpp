#include "JZScene.h"
#include "JZShader.h"
#include "JZTexture.h"
#include "JZDevice.h"

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

JZScene::JZScene():camera(glm::vec3(0.0f, 0.0f, 5.0f))
{
	m_pShader = new JZShader();
	m_pTexture = new JZTexture();
	m_pDevice = new JZDevice();
	downPoint = glm::vec2(0.0f, 0.0f);
	upPoint = glm::vec2(0.0f, 0.0f);
	memset(keys, 0, sizeof(bool)*1024);// ���̰�����״̬��true��ʾ���£�false��ʾ�ɿ�
	//lastFrameTime = 0;// ��һ֡��ʱ��
	deltaTime = 0.0f;// ��֡����һ֡��ʱ���

	lastFramePoint = glm::vec2(0.0f, 0.0f);
	firstCursor = true;
	isCSModel = false;

	groundColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
	pointColor = glm::vec3(0.6f, 0.0f, 0.0f);
	modelMatrix = glm::mat4();
	viewMatrix = camera.GetViewMatrix();
	projectionMatrix = glm::mat4();
}

JZScene::~JZScene()
{
	if (NULL != m_pShader)
	{
		delete (JZShader*)m_pShader;
		m_pShader = NULL;
	}
}

/////////////////////////////1.��ʼ�����ֵĺ���///////////////////////////////////////
void JZScene::SetHWnd(HWND cwnd)
{
	m_hwnd = cwnd;

	// �����괰�ڻ�ؼ�����󣬾�Ӧ�����ϸ���ͶӰ����
	_UpdateViewAndProjectionMatrix();
}

void JZScene::SetGroundColor(glm::vec4 color)
{
	groundColor = color;
}

void JZScene::SetPointColor(glm::vec3 color)
{
	pointColor = color;
}

void JZScene::InitMatrix()
{
	modelMatrix = glm::mat4();
	viewMatrix = glm::mat4();
	projectionMatrix = glm::mat4();
}

void JZScene::Reset()
{
	//camera.Reset();
	downPoint = glm::vec2(0.0f, 0.0f);
	upPoint = glm::vec2(0.0f, 0.0f);
	memset(keys, 0, sizeof(bool)*1024);// ���̰�����״̬��true��ʾ���£�false��ʾ�ɿ�
	//lastFrameTime = 0;// ��һ֡��ʱ��
	deltaTime = 0.0f;// ��֡����һ֡��ʱ���

	lastFramePoint = glm::vec2(0.0f, 0.0f);
	firstCursor = true;
	isCSModel = false;

	modelMatrix = glm::mat4();
}

void JZScene::PrepareData()
{
	// ��1��shader
	const char* shaderPath[2] = { "../../sys/shaders/texture.vert", "../../sys/shaders/texture.frag" };
	int iShaderNums = 2;
	m_pShader->CreateShaderProgram(shaderPath, iShaderNums);

	// ��2���������ꡢ��ɫ����������
	GLfloat vertices[] = 
	{
		//-----λ��-----		-----��ɫ-----		---��������---
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // ����
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // ����
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // ����
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // ����
	};

	for (int i = 0; i < 4; i++)
	{
		Vertex vertex;
		vertex.position.x = vertices[i * 8 + 0];
		vertex.position.y = vertices[i * 8 + 1];
		vertex.position.z = vertices[i * 8 + 2];
		vertex.color.r = vertices[i * 8 + 3];
		vertex.color.g = vertices[i * 8 + 4];
		vertex.color.b = vertices[i * 8 + 5];
		vertex.texture.x = vertices[i * 8 + 6];
		vertex.texture.y = vertices[i * 8 + 7];
		AddPoint(vertex);
	}

	// ��3��������Դ
	int texWidth = 0;
	int texHeight = 0;
	unsigned char* image = SOIL_load_image("../../sys/images/awesomeface.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGB);
	JZImageBuf imageBuf = { 0 };
	imageBuf.color = image;
	imageBuf.pitch = texWidth * 3;
	imageBuf.pixel_fmt = JZ_PIXFMT_RGB;
	m_pTexture->Create(texWidth, texHeight);
	m_pTexture->FillImage(&imageBuf);
	SOIL_free_image_data(image);
}

void JZScene::PushDataToGPU()
{

	GLfloat* vertices2 = new GLfloat[pointArray.size()*8];
	for (unsigned int i = 0; i < pointArray.size(); i++)
	{
		vertices2[i*8 + 0] = pointArray[i].position.x;
		vertices2[i*8 + 1] = pointArray[i].position.y;
		vertices2[i*8 + 2] = pointArray[i].position.z;
		vertices2[i*8 + 3] = pointArray[i].color.r;
		vertices2[i*8 + 4] = pointArray[i].color.g;
		vertices2[i*8 + 5] = pointArray[i].color.b;
		vertices2[i*8 + 6] = pointArray[i].texture.x;
		vertices2[i*8 + 7] = pointArray[i].texture.y;
	}

	GLuint verticesIndex[] =
	{
		0,1,3,
		1,2,3
	};

	GLenum eRet = 0;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	// ���Դ��д�����
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, pointArray.size()*8*sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (const void*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GLfloat), (const void*)(6*sizeof(GLfloat)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(verticesIndex), verticesIndex, GL_STATIC_DRAW);

	glBindVertexArray(0);
	eRet = glGetError();
	assert(0 == eRet);

	// ��shader�д�����
	m_pShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pTexture->GetTexID());
	glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), "fTexture"), 0);
	eRet = glGetError();
	assert(0 == eRet);

	delete[] vertices2;
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	eRet = glGetError();
	assert(0 == eRet);
}

JZ_RESULT JZScene::InitOpenGL()
{
	m_pDevice->CreateDevice(m_hwnd);

	RECT rect; //����������л�ͼ  
	GetClientRect(m_hwnd, &rect);
	//��������ռ���������ڴ�С 
	GLsizei width = rect.right - rect.left;
	GLsizei height = rect.bottom - rect.top;
	glViewport(0 , 0 , width, height);
	GLenum eRet = glGetError();
	assert(0 == eRet);  

	return JZ_SUCCESS;
}

/////////////////////////////2.���Ʋ��ֵĺ���////////////////////////////////////////
void JZScene::AddPoint(Vertex point)// ��ӵ�
{
	/*std::vector<Vertex>::iterator it;
	for (it = pointArray.begin(); it < pointArray.end(); it++)
	{
		if (it->position == point.position)
			break;
	}*/

	//if (pointArray.size() == 0 || it == pointArray.end())
	//{
		pointArray.push_back(point);
	//}
}
void JZScene::DeletePoint(glm::vec3 point)// ɾ����
{

}

void JZScene::RenderScene()
{
	GLenum eRet = 0;
	//////////////////////////////////////////////////////////////////////
	// ��¼��ǰ֡��ʱ��
	LARGE_INTEGER frequency, currentFrame;
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&currentFrame) ;
	float currentFrameTime = (float)(currentFrame.QuadPart)/frequency.QuadPart;
	deltaTime = currentFrameTime - lastFrameTime;
	lastFrameTime = currentFrameTime;

	glClearColor(groundColor.r, groundColor.g, groundColor.b, groundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	eRet = glGetError();
	assert(0 == eRet);


	////////////////////////////// ��shader�д�ģ�͡���ͼ��ͶӰ����//////////////////////////////
	//// ����ģ�͡���ͼ��ͶӰ����
	//_KeyControlCameraAndModel();	// ���̿����ӽǣ�Ҳ���Ǹı���ͼ��ͶӰ����
	//_MouseControlModel();		// ������ģ����ת

	//// ��shader�д�ģ�;���
	//pShader->Use();// ����shader
	//glUniformMatrix4fv(glGetUniformLocation(pShader->GetProgramID(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//eRet = glGetError();
	//assert(0 == eRet);

	//// ��shader�д���ͼ����
	//glUniformMatrix4fv(glGetUniformLocation(pShader->GetProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//eRet = glGetError();
	//assert(0 == eRet);

	//// ��shader�д�ͶӰ����
	//glUniformMatrix4fv(glGetUniformLocation(pShader->GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	//eRet = glGetError();
	//assert(0 == eRet);

	///////////////////////////////��shader�д�uniform����////////////////////////////////
	//glUniform3fv(glGetUniformLocation(pShader->GetProgramID(), "pointColor"), 1, glm::value_ptr(pointColor));
	//eRet = glGetError();
	//assert(0 == eRet);

	////////////////////////////////////����ͼ��////////////////////////////////////////
	glBindVertexArray(m_VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);

	/*glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, pointArray.size());*/
	glBindVertexArray(0);
	eRet = glGetError();
	assert(0 == eRet);
	SwapBuffers(wglGetCurrentDC()); 
}

/////////////////////////////3.��Ӧ���Ͱ������ֵĺ���///////////////////////////////////
void JZScene::SetDownPoint(glm::vec2 point)// ��������������ʱ��������ڵ�����
{
	if (!isCSModel && _IsCursorInRect())
	{
		downPoint = point;
		upPoint = downPoint;
	}
}

void JZScene::SetUpPoint(glm::vec2 point)// �����������ɿ�ʱ��������ڵ�����
{
	if (!isCSModel && _IsCursorInRect())
	{
		upPoint = point;
	}
}

void JZScene::SetKeyState(UINT nChar, bool state, bool resetAll/*= false*/)// ���ü��̰�����״̬
{
	if (_IsCursorInRect())
	{
		keys[nChar] = state;
	}
	else
		memset(keys, 0, sizeof(bool)*1024);

}

void JZScene::SwitchViewModel()
{
	/*if (_IsCursorInRect())
	{
		if (isCSModel)
		{
			isCSModel = false;
			ShowCursor(true);
			ClipCursor(NULL);
		}
		else
		{
			isCSModel = true;
			CPoint point;
			CRect rect;
			GetCursorPos(&point);
			::GetWindowRect(m_hwnd, rect);
			ShowCursor(false);
			ClipCursor(rect);
		}
	}*/
}

// �������currentPoint������ڿͻ���������λ��
void JZScene::MouseControlView(POINT currentPoint)
{
	
	//if (isCSModel && _IsCursorInRect())
	//{
	//	CRect rect;
	//	::GetClientRect(m_hwnd, rect);
	//	if (firstCursor)
	//	{
	//		lastFramePoint.x = (float)rect.CenterPoint().x;
	//		lastFramePoint.y = (float)rect.CenterPoint().y;
	//		firstCursor = false;
	//	}

	//	GLfloat xoffset = currentPoint.x - lastFramePoint.x;
	//	GLfloat yoffset = lastFramePoint.y - currentPoint.y;// ע���������෴�ģ���Ϊy����ķ�Χ�Ǵ������ϵ�
	//	lastFramePoint.x = (GLfloat)currentPoint.x;
	//	lastFramePoint.y = (GLfloat)currentPoint.y;

	//	GLfloat sensitivity = 0.2f;
	//	xoffset *= sensitivity;
	//	yoffset *= sensitivity;

	//	/*CPoint point;
	//	GetCursorPos(&point);
	//	CRect rrect;
	//	::GetWindowRect(m_hwnd, rrect);
	//	if (point.x <= (rrect.TopLeft().x + 1))
	//	{
	//	float velocity = 3.0f * 0.2f;
	//	xoffset = -velocity;
	//	}*/
	//	
	//	camera.Yaw += xoffset;
	//	camera.Pitch += yoffset;// ��Ϊ�����ƶ���yoffsetΪ��ֵ����������Ӧ������
	//	if (camera.Pitch > 89.0f)
	//		camera.Pitch = 89.0f;
	//	if (camera.Pitch < -89.0f)
	//		camera.Pitch = -89.0f;

	//	glm::vec3 front;
	//	front.x = cos(glm::radians(camera.Pitch)) * cos(glm::radians(camera.Yaw));
	//	front.y = sin(glm::radians(camera.Pitch));
	//	front.z = cos(glm::radians(camera.Pitch)) * sin(glm::radians(camera.Yaw));
	//	camera.Front = glm::normalize(front);

	//	_UpdateViewAndProjectionMatrix();
	//}
}

void JZScene::MouseWheelControlProjection(short zDelta, POINT point)
{

	/*if (_IsCursorInRect())
	{
		camera.ProcessMouseScroll((float)zDelta/WHEEL_DELTA);
	}

	_UpdateViewAndProjectionMatrix();

	if (_IsCursorInRect())
	{
		CRect rect;
		::GetClientRect(m_hwnd, rect);
		CPoint centerPoint = rect.CenterPoint();
		float x = (point.x - centerPoint.x) / (float)(rect.Width()/2);
		float y = (centerPoint.y - point.y) / (float)(rect.Height()/2);
		float z = 0;
	}*/
}

void JZScene::_UpdateViewAndProjectionMatrix()
{
	//// ������ͼ����
	//viewMatrix = camera.GetViewMatrix();

	//// ����ͶӰ����
	//CRect rect;
	//GetClientRect(m_hwnd, rect);
	//projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (GLfloat)rect.Width()/(GLfloat)rect.Height(), 0.1f, 100.0f);
}

bool JZScene::_IsCursorInRect()
{
	/*CPoint point;
	CRect rect;
	GetCursorPos(&point);
	::GetWindowRect(m_hwnd, rect);
	return rect.PtInRect(point);*/
	return true;
}

void JZScene::_KeyControlCameraAndModel()// ��סW��A��S��Dʱ������ӽǽ�����Ӧ�ı�
{
	if (isCSModel)
	{
		if(keys['w'] | keys['W'])
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if(keys['s'] | keys['S'])
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if(keys['a'] | keys['A'])
			camera.ProcessKeyboard(LEFT, deltaTime);
		if(keys['d'] | keys['D'])
			camera.ProcessKeyboard(RIGHT, deltaTime);

		_UpdateViewAndProjectionMatrix();
	} 
	else
	{
		glm::mat3 matrix = glm::mat3(modelMatrix);
		matrix = glm::inverse(matrix);
		float velocity = camera.MovementSpeed * deltaTime;

		// ����ģ�;���
		if(keys['w'] | keys['W'])
			modelMatrix = glm::translate(modelMatrix, matrix* glm::vec3(0.0f, velocity, 0.0f));
		if(keys['s'] | keys['S'])
			modelMatrix = glm::translate(modelMatrix, matrix* glm::vec3(0.0f, -velocity, 0.0f));
		if(keys['a'] | keys['A'])
			modelMatrix = glm::translate(modelMatrix, matrix* glm::vec3(-velocity, 0.0f, 0.0f));
		if(keys['d'] | keys['D'])
			modelMatrix = glm::translate(modelMatrix, matrix* glm::vec3(velocity, 0.0f, 0.0f));

		_UpdateViewAndProjectionMatrix();

	}
}

void JZScene::_MouseControlModel()
{
	if (_IsCursorInRect())
	{
		float rotateY = (upPoint.x - downPoint.x)/10;
		float rotateX = (upPoint.y - downPoint.y)/10;
		upPoint = downPoint;

		glm::mat3 matrix = glm::mat3(modelMatrix);
		matrix = glm::inverse(matrix);
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateY), matrix * glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(rotateX), matrix * glm::vec3(1.0f, 0.0f, 0.0f));
	}

}

////////////////////////////4.�ͷ��ڴ沿�ֵĺ���///////////////////////////////////

extern "C" _declspec(dllexport) IJZScene* GetSceneAPI()
{
	static JZScene scene;
	return &scene;
}