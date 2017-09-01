#include "JZUIEngine.h"
#include <IJZBaseImageProcessProc.h>
#include <IJZImageSmoothProc.h>
#include <IJZBaseRenderProc.h>

JZUIEngine::JZUIEngine()
{
	m_pSceneRender = NULL;
	m_imageProcessData.pSrcImage = NULL;
	m_imageProcessData.pDesImage = NULL;
	m_pBaseImageProcess = NULL;
	m_curPocessType = JZ_IMAGE_UNKNOW;
}

JZUIEngine::~JZUIEngine()
{

}

JZ_RESULT JZUIEngine::Init(HWND hWnd)
{
	JZ_RESULT res = JZ_UNKNOW;
	res = _InitRenderSDK(hWnd);
	res = _InitBaseImageSDK();
	_InitImageProcessPlugin();
	return res;
}

JZ_RESULT JZUIEngine::Release()
{
	_ReleaseRenderSDK();
	_ReleaseBaseImageSDK();
	_ReleaseImageProcessPlugin();
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::SetImageData(const char* filename)
{
	_ReleaseImageData();// 设置前，先释放没有释放的图像数据

	JZ_RESULT res = JZ_SUCCESS;
	if (NULL == filename)
	{
		return JZ_FAILED;
	}
	res = m_pBaseImageProcess->ReadImage(filename, m_imageProcessData.pSrcImage);
	if (JZ_SUCCESS == res)
	{
		m_imageProcessData.pDesImage->width = m_imageProcessData.pSrcImage->width;
		m_imageProcessData.pDesImage->height = m_imageProcessData.pSrcImage->height;
		m_imageProcessData.pDesImage->pixel_fmt = m_imageProcessData.pSrcImage->pixel_fmt;
		m_imageProcessData.pDesImage->color = new unsigned char[m_imageProcessData.pSrcImage->height *  m_imageProcessData.pSrcImage->pitch];
		m_imageProcessData.pDesImage->pitch = m_imageProcessData.pSrcImage->pitch;
		m_pSceneRender->SetLeftImage(m_imageProcessData.pSrcImage);
		m_pSceneRender->SetRightImage(NULL);
	}
	
	return JZ_SUCCESS;
}

// 将处理后的图像数据保存为图片
JZ_RESULT JZUIEngine::SaveImageData(const char* filename)
{
	JZ_RESULT res = m_pBaseImageProcess->WriteImage(m_imageProcessData.pDesImage, filename);
	return res;
}

JZ_RESULT JZUIEngine::SetProcessParam(JZCommonParam* pParam)
{
	switch (pParam->processType)
	{
	case JZ_IMAGE_SMOOTH:
	{
		JZSmoothParam* pSmoothParam = (JZSmoothParam*)pParam;
		((JZSmoothParam*)m_mapProcessParam[JZ_IMAGE_SMOOTH])->smoothType = pSmoothParam->smoothType;
		break;
	}
	default:
		break;
	}
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::SetCurProcessType(JZ_IMAGEPROC_TYPE curProcessType)
{
	m_curPocessType = curProcessType;
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::ProcessImage()
{
	if (JZ_IMAGE_UNKNOW == m_curPocessType)
	{
		return JZ_FAILED;
	}

	m_mapImageProcess[m_curPocessType]->ProcessImage(&m_imageProcessData, m_mapProcessParam[m_curPocessType]);
	m_pSceneRender->SetRightImage(m_imageProcessData.pDesImage);
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::Render()
{
	m_pSceneRender->RenderScene();
	return JZ_SUCCESS;
}

//////////////////////////////////////////// 私有函数//////////////////////////////////
JZ_RESULT JZUIEngine::_InitRenderSDK(HWND hWnd)
{
	m_pSceneRender = NULL;
	g_JZBaseRenderAPI->pfnGetSceneInterface(&m_pSceneRender);
	m_pSceneRender->init(hWnd);
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_ReleaseRenderSDK()
{
	if (NULL != m_pSceneRender)
	{
		g_JZBaseRenderAPI->pfnReleaseSceneInterface(m_pSceneRender);
		m_pSceneRender = NULL;
	}

	return JZ_SUCCESS;
}


JZ_RESULT JZUIEngine::_InitBaseImageSDK()
{
	// 初始化图像处理基类接口
	m_pBaseImageProcess = NULL;
	g_JZBaseImageProcessAPI->pfnGetInterface(&m_pBaseImageProcess);

	// 初始化图像数据
	static JZImageBuf src = { 0 };
	static JZImageBuf des = { 0 };
	m_imageProcessData.pSrcImage = &src;
	m_imageProcessData.pDesImage = &des;

	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_ReleaseBaseImageSDK()
{
	if (NULL != m_pBaseImageProcess)
	{
		_ReleaseImageData();		// 防止用户忘记释放图像数据
		g_JZImageSmoothAPI->pfnReleaseInterface(m_pBaseImageProcess);
		m_pBaseImageProcess = NULL;
	}

	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_InitImageProcessPlugin()
{
	// 初始化图像处理接口，并放入m_mapImageProcess
	IJZBaseImageProcess* tempBaseImageProcess = NULL;
	if (g_JZImageSmoothAPI)
	{
		g_JZImageSmoothAPI->pfnGetInterface(&tempBaseImageProcess);	// 生成图像平滑接口
		m_mapImageProcess.insert(pair<JZ_IMAGEPROC_TYPE, IJZBaseImageProcess*>(JZ_IMAGE_SMOOTH, tempBaseImageProcess));

		// 初始图像处理参数，并放入m_mapProcessParam
		JZSmoothParam* param = new JZSmoothParam();
		m_mapProcessParam.insert(pair<JZ_IMAGEPROC_TYPE, JZCommonParam*>(JZ_IMAGE_SMOOTH, param));
	}
	

	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_ReleaseImageProcessPlugin()
{
	// 释放图像处理接口
	for (map<JZ_IMAGEPROC_TYPE, IJZBaseImageProcess*>::iterator it = m_mapImageProcess.begin();
	it != m_mapImageProcess.end();)
	{
		if (NULL != it->second)
		{
			_ReleaseImageProcessAPI(it->first, it->second);
			it->second = NULL;
			it = m_mapImageProcess.erase(it);
		}
		else
		{
			it++;
		}
	}

	// 释放图像处理参数
	for (map<JZ_IMAGEPROC_TYPE, JZCommonParam*>::iterator it = m_mapProcessParam.begin();
	it != m_mapProcessParam.end();)
	{
		if (NULL != it->second)
		{
			delete it->second;
			it->second = NULL;
			it = m_mapProcessParam.erase(it);
		}
		else
		{
			it++;
		}
	}
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_ReleaseImageData()
{
	if (NULL != m_imageProcessData.pDesImage->color)
	{
		delete[] m_imageProcessData.pDesImage->color;
		m_imageProcessData.pDesImage->color = NULL;
	}

	if (NULL != m_imageProcessData.pSrcImage)
	{
		m_pBaseImageProcess->ReleaseImage(m_imageProcessData.pSrcImage);
		m_imageProcessData.pSrcImage->color = NULL;
	}

	return JZ_SUCCESS;
}

void JZUIEngine::_ReleaseImageProcessAPI(JZ_IMAGEPROC_TYPE eImageProcType, IJZBaseImageProcess*& pBaseImageProcess)
{
	switch (eImageProcType)
	{
	case JZ_IMAGE_SMOOTH:
		g_JZImageSmoothAPI->pfnReleaseInterface(pBaseImageProcess);
		pBaseImageProcess = NULL;
		break;
	default:
		break;
	}
}

//////////////////////////////////////////// 接口导出//////////////////////////////////
JZ_RESULT GetInterface(IJZUIEngine** ppAPI)
{
	*ppAPI = new JZUIEngine();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZUIEngine*& pAPI)
{
	if (NULL != pAPI)
	{
		delete (JZUIEngine*)pAPI;
		pAPI = NULL;
	}
	return JZ_SUCCESS;
}

JZUIEngineAPI* g_pJZUIEngineAPI;
extern "C" _declspec(dllexport) void* JZUIE_GetAPIStuPtr()
{
	static JZUIEngineAPI temp = { 0 };
	g_pJZUIEngineAPI = &temp;
	g_pJZUIEngineAPI->pfnGetInterface = GetInterface;
	g_pJZUIEngineAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pJZUIEngineAPI;
}
