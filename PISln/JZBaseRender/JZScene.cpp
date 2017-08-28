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
	m_pTexture = new JZTexture();
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
	m_pDevice->Create(hWnd);

	return JZ_SUCCESS;
}

void JZScene::SetImage(JZImageBuf* pImageBuf)
{	
	if (NULL == pImageBuf)
	{
		m_pTexture->Release();
		int texWidth = 1;
		int texHeight = 1;
		unsigned char* image = new unsigned char[3];
		image[0] = m_groundColor.r * 100;
		image[1] = m_groundColor.g * 100;
		image[2] = m_groundColor.b * 100;

		JZImageBuf imageBuf = { 0 };
		imageBuf.color = image;
		imageBuf.pitch = texWidth * 3;
		imageBuf.pixel_fmt = JZ_PIXFMT_RGB;
		imageBuf.width = texWidth;
		imageBuf.height = texHeight;
		m_pTexture->Create(&imageBuf);
	}
	else
	{
		m_pTexture->Release();
		m_pTexture->Create(pImageBuf);
	}
	
}

void JZScene::SetGroundColor(glm::vec4 color)
{
	m_groundColor = color;
}

void JZScene::PrepareData()
{
	// 【1】shader资源
	const char* shaderPath[2] = { "../../sys/shaders/texture.vert", "../../sys/shaders/texture.frag" };
	int iShaderNums = 2;
	m_pShader->Create(shaderPath, iShaderNums);

	// 【2】纹理资源
	int texWidth = 1;
	int texHeight = 1;
	unsigned char* image = new unsigned char[3];
	image[0] = m_groundColor.r * 100;
	image[1] = m_groundColor.g * 100;
	image[2] = m_groundColor.b * 100;
	
	JZImageBuf imageBuf = { 0 };
	imageBuf.color = image;
	imageBuf.pitch = texWidth * 3;
	imageBuf.pixel_fmt = JZ_PIXFMT_RGB;
	imageBuf.width = texWidth;
	imageBuf.height = texHeight;
	m_pTexture->Create(&imageBuf);

	// 【3】网格资源
	m_pMesh->CreateQuadMesh();

	// 【4】帧缓存资源
	GLuint srcRenderbuffer;
	glGenRenderbuffers(1, &srcRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, srcRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, 395, 487);

	glGenFramebuffers(1, &m_srcFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_srcFramebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, srcRenderbuffer);

	GLenum srcFrameStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != srcFrameStatus)
	{
		assert(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLuint dstRenderbuffer;
	glGenRenderbuffers(1, &dstRenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, dstRenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGB, 395, 487);

	glGenFramebuffers(1, &m_dstFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_dstFramebuffer);
	glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, dstRenderbuffer);

	GLenum dstFrameStatus = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
	if (GL_FRAMEBUFFER_COMPLETE != dstFrameStatus)
	{
		assert(0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	m_bResChanged = true;
}

void JZScene::SetResStatus(bool bHasChanged /*=true*/)
{
	m_bResChanged = bHasChanged;
}

void JZScene::RenderScene()
{
	// 未经图像处理的buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_srcFramebuffer);
	glViewport(0, 0, 395, 487);
	glClearColor(m_groundColor.r, m_groundColor.g, m_groundColor.b, m_groundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	assert(0 == glGetError());

	if (m_bResChanged)
	{
		// 往shader中传纹理
		m_pShader->Use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pTexture->GetTexID());
		glUniform1i(glGetUniformLocation(m_pShader->GetProgramID(), "fTexture"), 0);
		assert(0 == glGetError());
		m_bResChanged = false;

		// 往shader中传其他参数
		// todo...
	}

	m_pMesh->Draw();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_srcFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 395, 487, 0, 0, 395, 487, GL_COLOR_BUFFER_BIT, GL_NEAREST);

	// 经过图像处理的buffer
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_dstFramebuffer);
	glViewport(0, 0, 395, 487);
	glClearColor(m_groundColor.r, m_groundColor.g, m_groundColor.b, m_groundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	assert(0 == glGetError());
	m_pMesh->Draw();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_dstFramebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 395, 487, 396, 0, 791, 487, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	SwapBuffers(wglGetCurrentDC()); 
}

void JZScene::Release()
{
	if (NULL != m_pShader)
	{
		delete (JZShader*)m_pShader;
		m_pShader = NULL;
	}
	
	if (NULL != m_pTexture)
	{
		delete (JZTexture*)m_pTexture;
		m_pTexture = NULL;
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


///////////////////////////////////接口导出////////////////////////////////
extern "C" _declspec(dllexport) IJZScene* GetSceneAPI()
{
	static JZScene scene;
	return &scene;
}

JZ_RESULT GetSceneInterface(IJZScene** ppScene)
{
	*ppScene = new JZScene();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseSceneInterface(IJZScene* pScene)
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