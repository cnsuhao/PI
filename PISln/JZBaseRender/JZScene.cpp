#include "JZScene.h"
#include "JZShader.h"
Scene::Scene():camera(glm::vec3(0.0f, 0.0f, 5.0f))
{
	pShader = new JZShader();
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

Scene::~Scene()
{
	if (NULL != pShader)
	{
		delete (JZShader*)pShader;
		pShader = NULL;
	}
}

/////////////////////////////1.初始化部分的函数///////////////////////////////////////
void Scene::SetHWnd(HWND cwnd)
{
	m_hwnd = cwnd;

	// 设置完窗口或控件句柄后，就应该马上更新投影矩阵
	_UpdateViewAndProjectionMatrix();
}

void Scene::SetGroundColor(glm::vec4 color)
{
	groundColor = color;
}

void Scene::SetPointColor(glm::vec3 color)
{
	pointColor = color;
}

void Scene::InitMatrix()
{
	modelMatrix = glm::mat4();
	viewMatrix = glm::mat4();
	projectionMatrix = glm::mat4();
}

void Scene::Reset()
{
	camera.Reset();
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

void Scene::PrepareData()
{
	// 【1】shader
	const char* shaderPath[2] = { "../sys/shaders/coordSystem.vert", "../sys/shaders/coordSystem.frag" };
	int iShaderNums = 2;
	pShader->CreateShaderProgram(shaderPath, iShaderNums);

	// 【2】顶点坐标、颜色、纹理坐标
	//GLfloat vertices[] = 
	//{
	//	//-----位置-----		---纹理坐标---
	//	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,
	//	1.0f, -1.0f, -1.0f,  1.0f, 0.0f,
	//	1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	//	1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	//	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,

	//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	//	1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	//	1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
	//	1.0f,  1.0f,  1.0f,  1.0f, 1.0f,
	//	-1.0f,  1.0f,  1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,

	//	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	//	-1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	//	-1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

	//	1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	//	1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	//	1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	//	1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	//	1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	//	1.0f,  1.0f,  1.0f,  1.0f, 0.0f,

	//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,
	//	1.0f, -1.0f, -1.0f,  1.0f, 1.0f,
	//	1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	//	1.0f, -1.0f,  1.0f,  1.0f, 0.0f,
	//	-1.0f, -1.0f,  1.0f,  0.0f, 0.0f,
	//	-1.0f, -1.0f, -1.0f,  0.0f, 1.0f,

	//	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f,
	//	1.0f,  1.0f, -1.0f,  1.0f, 1.0f,
	//	1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	//	1.0f,  1.0f,  1.0f,  1.0f, 0.0f,
	//	-1.0f,  1.0f,  1.0f,  0.0f, 0.0f,
	//	-1.0f,  1.0f, -1.0f,  0.0f, 1.0f
	//};

	//for (int i = 0; i < 36; i++)
	//{
	//	Vertex vertex;
	//	vertex.position.x = vertices[i*5 + 0];
	//	vertex.position.y = vertices[i*5 + 1];
	//	vertex.position.z = vertices[i*5 + 2];
	//	vertex.color.r = 1.0f;
	//	vertex.color.g = 1.0f;
	//	vertex.color.b = 1.0f;
	//	AddPoint(vertex);
	//}

	//Vertex largePoint;
	//largePoint.position = glm::vec3(0.0f, 0.0f, 0.0f);
	//largePoint.color = glm::vec3(1.0f, 1.0f, 1.0f);
	//AddPoint(largePoint);
}

void Scene::PushDataToGPU()
{

	GLfloat* vertices2 = new GLfloat[pointArray.size()*6];
	for (unsigned int i = 0; i < pointArray.size(); i++)
	{
		vertices2[i*6 + 0] = pointArray[i].position.x;
		vertices2[i*6 + 1] = pointArray[i].position.y;
		vertices2[i*6 + 2] = pointArray[i].position.z;
		vertices2[i*6 + 3] = pointArray[i].color.r;
		vertices2[i*6 + 4] = pointArray[i].color.g;
		vertices2[i*6 + 5] = pointArray[i].color.b;
	}

	GLenum eRet = 0;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// 往显存中存数据
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, pointArray.size()*6*sizeof(GLfloat), vertices2, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), NULL);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (const void*)(3*sizeof(GLfloat)));
	glBindVertexArray(0);
	eRet = glGetError();
	assert(0 == eRet);

	delete[] vertices2;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	eRet = glGetError();
	assert(0 == eRet);
}

bool Scene::InitOpenGL()
{
	GLenum eRet = 0;	
	static PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR),  
		1,  
		PFD_DRAW_TO_WINDOW |  
		PFD_SUPPORT_OPENGL |  
		PFD_DOUBLEBUFFER,  
		PFD_TYPE_RGBA,  
		24 ,  
		0, 0, 0, 0, 0, 0,  
		0,  
		0,  
		0,  
		0, 0, 0, 0,  
		32 ,  
		0,  
		0,  
		PFD_MAIN_PLANE,  
		0,  
		0, 0, 0  
	};  
	int pixelformat;

	m_hDC = ::GetDC(m_hwnd);  
	if (!(pixelformat = ChoosePixelFormat(m_hDC , &pfd)))  
	{   
		return false;  
	}  
	if (!SetPixelFormat(m_hDC , pixelformat , &pfd))  
	{   
		return false;  
	}  
	if (!(m_hRC = wglCreateContext(m_hDC)))  
	{  
		return false;  
	}  
	if (!wglMakeCurrent(m_hDC , m_hRC))  
	{  
		return false;  
	}     

	glewExperimental = GL_TRUE;
	glewInit();
	eRet = glGetError();
	assert(0 == eRet);  

	CRect rect; //在这个矩形中画图  
	GetClientRect(m_hwnd, rect);
	//绘制区域占据整个窗口大小  
	glViewport(0 , 0 , rect.Width() , rect.Height());
	eRet = glGetError();
	assert(0 == eRet);  

	return true;
}

/////////////////////////////2.绘制部分的函数////////////////////////////////////////
void Scene::AddPoint(Vertex point)// 添加点
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
void Scene::DeletePoint(glm::vec3 point)// 删除点
{

}

void Scene::RenderScene()
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


	//////////////////////////// 往shader中传模型、视图、投影矩阵//////////////////////////////
	// 控制模型、视图和投影矩阵
	_KeyControlCameraAndModel();	// 键盘控制视角，也就是改变视图、投影矩阵
	_MouseControlModel();		// 鼠标控制模型旋转

	// 向shader中传模型矩阵
	pShader.Use();// 启用shader
	glUniformMatrix4fv(glGetUniformLocation(pShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
	eRet = glGetError();
	assert(0 == eRet);

	// 向shader中传视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(pShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
	eRet = glGetError();
	assert(0 == eRet);

	// 向shader中传投影矩阵
	glUniformMatrix4fv(glGetUniformLocation(pShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	eRet = glGetError();
	assert(0 == eRet);

	/////////////////////////////往shader中传uniform变量////////////////////////////////
	glUniform3fv(glGetUniformLocation(pShader.Program, "pointColor"), 1, glm::value_ptr(pointColor));
	eRet = glGetError();
	assert(0 == eRet);

	////////////////////////////////////绘制图形////////////////////////////////////////
	glBindVertexArray(VAO);
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, pointArray.size());
	glBindVertexArray(0);
	eRet = glGetError();
	assert(0 == eRet);
	SwapBuffers(wglGetCurrentDC()); 
}

/////////////////////////////3.响应鼠标和按键部分的函数///////////////////////////////////
void Scene::SetDownPoint(glm::vec2 point)// 设置鼠标左键按下时，鼠标所在点坐标
{
	if (!isCSModel && _IsCursorInRect())
	{
		downPoint = point;
		upPoint = downPoint;
	}
}

void Scene::SetUpPoint(glm::vec2 point)// 设置鼠标左键松开时，鼠标所在点坐标
{
	if (!isCSModel && _IsCursorInRect())
	{
		upPoint = point;
	}
}

void Scene::SetKeyState(UINT nChar, bool state, bool resetAll/*= false*/)// 设置键盘按键的状态
{
	if (_IsCursorInRect())
	{
		keys[nChar] = state;
	}
	else
		memset(keys, 0, sizeof(bool)*1024);

}

void Scene::SwitchViewModel()
{
	if (_IsCursorInRect())
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
	}
}

// 输入参数currentPoint是鼠标在客户区域的相对位置
void Scene::MouseControlView(CPoint currentPoint)
{
	
	if (isCSModel && _IsCursorInRect())
	{
		CRect rect;
		::GetClientRect(m_hwnd, rect);
		if (firstCursor)
		{
			lastFramePoint.x = (float)rect.CenterPoint().x;
			lastFramePoint.y = (float)rect.CenterPoint().y;
			firstCursor = false;
		}

		GLfloat xoffset = currentPoint.x - lastFramePoint.x;
		GLfloat yoffset = lastFramePoint.y - currentPoint.y;// 注意这里是相反的，因为y坐标的范围是从下往上的
		lastFramePoint.x = (GLfloat)currentPoint.x;
		lastFramePoint.y = (GLfloat)currentPoint.y;

		GLfloat sensitivity = 0.2f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		/*CPoint point;
		GetCursorPos(&point);
		CRect rrect;
		::GetWindowRect(m_hwnd, rrect);
		if (point.x <= (rrect.TopLeft().x + 1))
		{
		float velocity = 3.0f * 0.2f;
		xoffset = -velocity;
		}*/
		
		camera.Yaw += xoffset;
		camera.Pitch += yoffset;// 因为往上移动，yoffset为负值，而俯仰角应该增大
		if (camera.Pitch > 89.0f)
			camera.Pitch = 89.0f;
		if (camera.Pitch < -89.0f)
			camera.Pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(camera.Pitch)) * cos(glm::radians(camera.Yaw));
		front.y = sin(glm::radians(camera.Pitch));
		front.z = cos(glm::radians(camera.Pitch)) * sin(glm::radians(camera.Yaw));
		camera.Front = glm::normalize(front);

		_UpdateViewAndProjectionMatrix();
	}
}

void Scene::MouseWheelControlProjection(short zDelta, CPoint point)
{

	if (_IsCursorInRect())
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
	}
}

void Scene::_UpdateViewAndProjectionMatrix()
{
	// 更新视图矩阵
	viewMatrix = camera.GetViewMatrix();

	// 更新投影矩阵
	CRect rect;
	GetClientRect(m_hwnd, rect);
	projectionMatrix = glm::perspective(glm::radians(camera.Zoom), (GLfloat)rect.Width()/(GLfloat)rect.Height(), 0.1f, 100.0f);
}

bool Scene::_IsCursorInRect()
{
	CPoint point;
	CRect rect;
	GetCursorPos(&point);
	::GetWindowRect(m_hwnd, rect);
	return rect.PtInRect(point);
}

void Scene::_KeyControlCameraAndModel()// 按住W、A、S、D时，相机视角进行相应改变
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

void Scene::_MouseControlModel()
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
void Scene::DestroyOpenGL()
{
	m_hRC = ::wglGetCurrentContext();  

	if(m_hRC)  
	{  
		::wglDeleteContext(m_hRC);
	}  

	if(m_hDC)  
	{  
		delete m_hDC;  
	}  
	m_hDC = NULL;  
}