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
	memset(keys, 0, sizeof(bool)*1024);// 键盘按键的状态，true表示按下，false表示松开
	//lastFrameTime = 0;// 上一帧的时刻
	deltaTime = 0.0f;// 本帧和上一帧的时间差

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

/////////////////////////////1.初始化部分的函数///////////////////////////////////////
void JZScene::SetHWnd(HWND cwnd)
{
	m_hwnd = cwnd;

	// 设置完窗口或控件句柄后，就应该马上更新投影矩阵
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
	memset(keys, 0, sizeof(bool)*1024);// 键盘按键的状态，true表示按下，false表示松开
	//lastFrameTime = 0;// 上一帧的时刻
	deltaTime = 0.0f;// 本帧和上一帧的时间差

	lastFramePoint = glm::vec2(0.0f, 0.0f);
	firstCursor = true;
	isCSModel = false;

	modelMatrix = glm::mat4();
}

void JZScene::PrepareData()
{
	// 【1】shader
	const char* shaderPath[2] = { "../../sys/shaders/texture.vert", "../../sys/shaders/texture.frag" };
	int iShaderNums = 2;
	m_pShader->CreateShaderProgram(shaderPath, iShaderNums);

	// 【2】顶点坐标、颜色、纹理坐标
	GLfloat vertices[] = 
	{
		//-----位置-----		-----颜色-----		---纹理坐标---
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 右上
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 右下
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 左下
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // 左上
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

	// 【3】纹理资源
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

	// 往显存中存数据
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

	// 往shader中传纹理
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

	RECT rect; //在这个矩形中画图  
	GetClientRect(m_hwnd, &rect);
	//绘制区域占据整个窗口大小 
	GLsizei width = rect.right - rect.left;
	GLsizei height = rect.bottom - rect.top;
	glViewport(0 , 0 , width, height);
	GLenum eRet = glGetError();
	assert(0 == eRet);  

	return JZ_SUCCESS;
}

/////////////////////////////2.绘制部分的函数////////////////////////////////////////
void JZScene::AddPoint(Vertex point)// 添加点
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
void JZScene::DeletePoint(glm::vec3 point)// 删除点
{

}

void JZScene::RenderScene()
{
	GLenum eRet = 0;
	//////////////////////////////////////////////////////////////////////
	// 记录当前帧的时间
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


	////////////////////////////// 往shader中传模型、视图、投影矩阵//////////////////////////////
	//// 控制模型、视图和投影矩阵
	//_KeyControlCameraAndModel();	// 键盘控制视角，也就是改变视图、投影矩阵
	//_MouseControlModel();		// 鼠标控制模型旋转

	//// 向shader中传模型矩阵
	//pShader->Use();// 启用shader
	//glUniformMatrix4fv(glGetUniformLocation(pShader->GetProgramID(), "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	//eRet = glGetError();
	//assert(0 == eRet);

	//// 向shader中传视图矩阵
	//glUniformMatrix4fv(glGetUniformLocation(pShader->GetProgramID(), "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	//eRet = glGetError();
	//assert(0 == eRet);

	//// 向shader中传投影矩阵
	//glUniformMatrix4fv(glGetUniformLocation(pShader->GetProgramID(), "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	//eRet = glGetError();
	//assert(0 == eRet);

	///////////////////////////////往shader中传uniform变量////////////////////////////////
	//glUniform3fv(glGetUniformLocation(pShader->GetProgramID(), "pointColor"), 1, glm::value_ptr(pointColor));
	//eRet = glGetError();
	//assert(0 == eRet);

	////////////////////////////////////绘制图形////////////////////////////////////////
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

/////////////////////////////3.响应鼠标和按键部分的函数///////////////////////////////////
void JZScene::SetDownPoint(glm::vec2 point)// 设置鼠标左键按下时，鼠标所在点坐标
{
	if (!isCSModel && _IsCursorInRect())
	{
		downPoint = point;
		upPoint = downPoint;
	}
}

void JZScene::SetUpPoint(glm::vec2 point)// 设置鼠标左键松开时，鼠标所在点坐标
{
	if (!isCSModel && _IsCursorInRect())
	{
		upPoint = point;
	}
}

void JZScene::SetKeyState(UINT nChar, bool state, bool resetAll/*= false*/)// 设置键盘按键的状态
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

// 输入参数currentPoint是鼠标在客户区域的相对位置
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
	//	GLfloat yoffset = lastFramePoint.y - currentPoint.y;// 注意这里是相反的，因为y坐标的范围是从下往上的
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
	//	camera.Pitch += yoffset;// 因为往上移动，yoffset为负值，而俯仰角应该增大
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
	//// 更新视图矩阵
	//viewMatrix = camera.GetViewMatrix();

	//// 更新投影矩阵
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

void JZScene::_KeyControlCameraAndModel()// 按住W、A、S、D时，相机视角进行相应改变
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

		// 更新模型矩阵
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

////////////////////////////4.释放内存部分的函数///////////////////////////////////

extern "C" _declspec(dllexport) IJZScene* GetSceneAPI()
{
	static JZScene scene;
	return &scene;
}