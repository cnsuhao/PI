#include "JZDevice.h"
#include "JZShader.h"
#include "JZTexture.h"
#include "JZMesh.h"
#include "JZScene.h"
#include <IJZBaseRender.h>

#pragma comment(lib, "opengl32.lib")
#ifdef _DEBUG
#	ifdef _WIN64
#		pragma comment(lib, "glew64d.lib")
#	else
#		pragma comment(lib, "glew32d.lib")
#	endif
#else
#	ifdef _WIN64
#		pragma comment(lib, "glew64.lib")
#	else
#		pragma comment(lib, "glew32.lib")
#	endif
#endif // _DEBUG


JZScene::JZScene()
{
	m_pShader = new JZShader();
	m_pSrcTexture = new JZTexture();
	m_pDstTexture = new JZTexture();
	m_pDevice = new JZDevice();
	m_pMesh = new JZMesh();
	m_groundColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f);
	m_bResChanged = true;
}

JZScene::~JZScene()
{
	Release();
}

JZ_RESULT JZScene::SetDevice(HWND hWnd)
{
	// 设备资源
	m_hwnd = hWnd;
	m_pDevice->Create(hWnd);

	// 帧缓存资源
	RECT rect; //在这个矩形中画图  
	GetClientRect(m_hwnd, &rect);
	//绘制区域占据整个窗口大小 
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;
	float texAspect = (float)m_pSrcTexture->GetTexWidth() / (float)m_pSrcTexture->GetTexHeight();
	float rectAspect = (float)((width - 1) / 2) / (float)height;

	glGenRenderbuffers(1, &m_srcRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_srcRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width / 2, height);

	glGenFramebuffers(1, &m_srcFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_srcFramebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_srcRenderbuffer);

	GLenum srcFrameStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != srcFrameStatus)
	{
		assert(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	glGenRenderbuffers(1, &m_dstRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_dstRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width / 2, height);

	glGenFramebuffers(1, &m_dstFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_dstFramebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_dstRenderbuffer);

	GLenum dstFrameStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != dstFrameStatus)
	{
		assert(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_bResChanged = true;

	return JZ_SUCCESS;
}

void JZScene::SetShader()
{
	// shader资源
	const char* shaderPath[2] = { "texture.vert", "texture.frag" };
	int iShaderNums = 2;
	m_pShader->Create(shaderPath, iShaderNums);
}

void JZScene::SetLeftImage(JZImageBuf* pImageBuf /* = NULL */)
{	// 纹理资源
	if (NULL == pImageBuf)
	{
		m_pSrcTexture->Release();
		int texWidth = 1;
		int texHeight = 1;
		unsigned char* image = new unsigned char[3];
		image[0] = m_groundColor.r * 255;
		image[1] = m_groundColor.g * 255;
		image[2] = m_groundColor.b * 255;

		JZImageBuf imageBuf = { 0 };
		imageBuf.color = image;
		imageBuf.pitch = texWidth * 3;
		imageBuf.pixel_fmt = JZ_PIXFMT_RGB;
		imageBuf.width = texWidth;
		imageBuf.height = texHeight;
		m_pSrcTexture->Create(&imageBuf);
	}
	else
	{
		m_pSrcTexture->Release();
		m_pSrcTexture->Create(pImageBuf);
	}

	_RenderSrcFramebuffer();

}

void JZScene::SetRightImage(JZImageBuf* pImageBuf /* = NULL */)
{	// 纹理资源
	if (NULL == pImageBuf)
	{
		m_pDstTexture->Release();
		int texWidth = 1;
		int texHeight = 1;
		unsigned char* image = new unsigned char[3];
		image[0] = m_groundColor.r * 255;
		image[1] = m_groundColor.g * 255;
		image[2] = m_groundColor.b * 255;

		JZImageBuf imageBuf = { 0 };
		imageBuf.color = image;
		imageBuf.pitch = texWidth * 3;
		imageBuf.pixel_fmt = JZ_PIXFMT_RGB;
		imageBuf.width = texWidth;
		imageBuf.height = texHeight;
		m_pDstTexture->Create(&imageBuf);
	}
	else
	{
		m_pDstTexture->Release();
		m_pDstTexture->Create(pImageBuf);
	}

	_RenderDstFramebuffer();
}

void JZScene::SetGroundColor(glm::vec4 color /*= glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)*/)
{
	m_groundColor = color;
}

void JZScene::init(HWND hWnd)
{
	SetDevice(hWnd);
	SetShader();
	SetLeftImage();
	SetRightImage();
}

void JZScene::RenderScene()
{
	_RenderSrcFramebuffer();
	_RenderDstFramebuffer();

	RECT rect; //在这个矩形中画图  
	GetClientRect(m_hwnd, &rect);
	//绘制区域占据整个窗口大小 
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_srcFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, (width-1) / 2, height, 0, 0, (width-1) / 2, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_dstFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, (width-1) / 2, height, (width+1) / 2, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	SwapBuffers(wglGetCurrentDC()); 
}

void JZScene::Release()
{
	if (NULL != m_pShader)
	{
		delete (JZShader*)m_pShader;
		m_pShader = NULL;
	}
	
	if (NULL != m_pSrcTexture)
	{
		delete (JZTexture*)m_pSrcTexture;
		m_pSrcTexture = NULL;
	}

	if (NULL != m_pDstTexture)
	{
		delete (JZTexture*)m_pDstTexture;
		m_pDstTexture = NULL;
	}

	if (NULL != m_pMesh)
	{
		delete (JZMesh*)m_pMesh;
		m_pMesh = NULL;
	}

	if (NULL != m_pDevice) // 注意OpenGL设备应该最后释放，因为其他资源都用到了OpenGL
	{
		delete (JZDevice*)m_pDevice;
		m_pDevice = NULL;
	}
}

JZ_RESULT JZScene::_RenderSrcFramebuffer()
{
	RECT rect; //在这个矩形中画图  
	GetClientRect(m_hwnd, &rect);
	//绘制区域占据整个窗口大小 
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 往shader中传纹理
	m_pShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pSrcTexture->GetTexID());
	glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), "fTexture"), 0);
	assert(0 == glGetError());

	// 往shader中传其他参数
	// todo...

	glBindRenderbuffer(GL_RENDERBUFFER, m_srcRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width / 2, height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_srcFramebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_srcRenderbuffer);
	glViewport(0, 0, (width - 1) / 2, height);
	glClearColor(m_groundColor.r, m_groundColor.g, m_groundColor.b, m_groundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	assert(0 == glGetError());
	float texAspect = (float)m_pSrcTexture->GetTexWidth() / (float)m_pSrcTexture->GetTexHeight();
	float rectAspect = (float)((width - 1) / 2) / (float)height;
	m_pMesh->CreateQuadMesh(rectAspect, texAspect);
	m_pMesh->Draw();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return JZ_SUCCESS;
}

JZ_RESULT JZScene::_RenderDstFramebuffer()
{
	RECT rect; //在这个矩形中画图  
	GetClientRect(m_hwnd, &rect);
	//绘制区域占据整个窗口大小 
	int width = rect.right - rect.left;
	int height = rect.bottom - rect.top;

	// 往shader中传纹理
	m_pShader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pDstTexture->GetTexID());
	glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), "fTexture"), 0);
	assert(0 == glGetError());

	// 往shader中传其他参数
	// todo...

	glBindRenderbuffer(GL_RENDERBUFFER, m_dstRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, width / 2, height);

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_dstFramebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, m_dstRenderbuffer);
	glViewport(0, 0, (width - 1) / 2, height);
	glClearColor(m_groundColor.r, m_groundColor.g, m_groundColor.b, m_groundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	assert(0 == glGetError());
	float texAspect = (float)m_pDstTexture->GetTexWidth() / (float)m_pDstTexture->GetTexHeight();
	float rectAspect = (float)((width - 1) / 2) / (float)height;
	m_pMesh->CreateQuadMesh(rectAspect, texAspect);
	m_pMesh->Draw();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	return JZ_SUCCESS;
}


///////////////////////////////////接口导出////////////////////////////////
extern "C" _declspec(dllexport) IJZSceneRender* GetSceneAPI()
{
	static JZScene scene;
	return &scene;
}

JZ_RESULT GetSceneInterface(IJZSceneRender** ppScene)
{
	*ppScene = new JZScene();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseSceneInterface(IJZSceneRender*& pScene)
{
	if (NULL != pScene)
	{
		delete (JZScene*)pScene;
		pScene = NULL;
	}
	return JZ_SUCCESS;
}

JZBaseRenderAPI* g_pBaseRenderAPI;
extern "C" _declspec(dllexport)void* JZBR_GetAPIStuPtr()
{
	static JZBaseRenderAPI temp = { 0 };
	g_pBaseRenderAPI = &temp;
	g_pBaseRenderAPI->pfnGetSceneInterface = GetSceneInterface;
	g_pBaseRenderAPI->pfnReleaseSceneInterface = ReleaseSceneInterface;
	return (void*)g_pBaseRenderAPI;
}