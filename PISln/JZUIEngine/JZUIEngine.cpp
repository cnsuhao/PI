#include "JZUIEngine.h"
#include <IJZBaseRenderProc.h>
#include <IJZBaseImageProcessProc.h>
#include <IJZImagePlateRecogProc.h>

JZUIEngine::JZUIEngine()
{
	m_pSceneRender = NULL;
	m_imageProcessData.pSrcImage = NULL;
	m_imageProcessData.pDesImage = NULL;
	m_pBaseImageProcess = NULL;
	m_pImagePlateRecog = NULL;
	m_bIsInit = false;
}

JZUIEngine::~JZUIEngine()
{

}

// 初始化UIEngine中成员
JZ_RESULT JZUIEngine::Init(HWND hWnd)
{
	JZ_RESULT res = JZ_UNKNOW;
	res = _InitRenderSDK(hWnd);
	res = _InitImageProcessSDK();
	m_bIsInit = true;
	return res;
}

// 释放UIEngine中成员
JZ_RESULT JZUIEngine::Release()
{
	_ReleaseRenderSDK();
	_ReleaseImageProcessSDK();
	return JZ_SUCCESS;
}

// 设置要处理的图像数据
JZ_RESULT JZUIEngine::SetImageData(const char* filename)
{
	if (!m_bIsInit)
	{
		return JZ_FAILED;
	}

	_ReleaseImageData();// 设置前，先释放没有释放的图像数据

	JZ_RESULT res = m_pBaseImageProcess->ReadImage(filename, m_imageProcessData.pSrcImage);
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
	else
	{
		m_pSceneRender->SetLeftImage(NULL);
		m_pSceneRender->SetRightImage(NULL);
	}

	m_imageProcessData.pExtraData = new JZImageProcessExtraData();
	
	return JZ_SUCCESS;
}

// 是否已经设置了要处理的图像
bool JZUIEngine::IsSetSrcImage()
{
	if (NULL == m_imageProcessData.pSrcImage->color)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// 将处理后的图像数据保存为图片
JZ_RESULT JZUIEngine::SaveImageData(const char* filename)
{
	JZ_RESULT res = m_pBaseImageProcess->WriteImage(m_imageProcessData.pDesImage, filename);
	return res;
}

// 设置图像处理的参数
JZ_RESULT JZUIEngine::SetProcessParam(JZCommonParam* pParam)
{
	switch (pParam->processType)
	{
	case JZ_IMAGE_BASEPROCESS: // 基本图像处理
	{
		JZBaseProcessParam* pBaseProcessParam = (JZBaseProcessParam*)pParam;
		switch (pBaseProcessParam->baseProcessType)
		{
		case JZ_BASEPROCESS_SMOOTH: // 图像平滑
		{
			JZSmoothParam* pSmoothParam = (JZSmoothParam*)pParam;
			((JZSmoothParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_SMOOTH])->smoothType = pSmoothParam->smoothType;
			break;
		}
		case JZ_BASEPROCESS_MORPHOLOGY: // 图像形态学
		{
			JZMorphologyParam* pMorphologyParam = (JZMorphologyParam*)pParam;
			((JZMorphologyParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_MORPHOLOGY])->morphologyType = pMorphologyParam->morphologyType;
			((JZMorphologyParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_MORPHOLOGY])->morphologyShape = pMorphologyParam->morphologyShape;
			((JZMorphologyParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_MORPHOLOGY])->width = pMorphologyParam->width;
			((JZMorphologyParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_MORPHOLOGY])->height = pMorphologyParam->height;
			break;
		}
		case JZ_BASEPROCESS_HISTOGRAM: // 图像直方图
		{
			break;
		}
		default:
			break;
		}
		break;
	}
	case JZ_IMAGE_PLATERECOG: // 车牌识别
	{
		break;
	}
	default:
		break;
	}
	return JZ_SUCCESS;
}

// 根据图像处理类型进行图像处理
JZ_RESULT JZUIEngine::ProcessImage(JZ_IMAGEPROC_TYPE processType, JZ_BASEPROCESS_TYPE baseProcessType/* = JZ_BASEPROCESS_UNKNOWN*/)
{
	JZ_RESULT res = JZ_UNKNOW;
	if (JZ_IMAGE_UNKNOWN == processType)
	{
		return JZ_FAILED;
	}

	switch (processType)
	{
	case JZ_IMAGE_BASEPROCESS:
		res = m_pBaseImageProcess->ProcessImage(&m_imageProcessData, m_mapBaseProcessParam[baseProcessType]);
		break;
	case JZ_IMAGE_PLATERECOG:
		res = m_pImagePlateRecog->ProcessImage(&m_imageProcessData, m_pPlateRecogParam);
		break;
	default:
		break;
	}

	m_pSceneRender->SetRightImage(m_imageProcessData.pDesImage);
	return res;
}

// 获取存储的额外图像处理结果数据
JZImageProcessExtraData* JZUIEngine::GetExtraData()
{
	return m_imageProcessData.pExtraData;
}

// 渲染场景
JZ_RESULT JZUIEngine::Render()
{
	if (!m_bIsInit)
	{
		return JZ_FAILED;
	}
	m_pSceneRender->RenderScene();
	return JZ_SUCCESS;
}

//////////////////////////////////////////// 私有函数//////////////////////////////////
// 初始化图形渲染接口
JZ_RESULT JZUIEngine::_InitRenderSDK(HWND hWnd)
{
	m_pSceneRender = NULL;
	g_JZBaseRenderAPI->pfnGetSceneInterface(&m_pSceneRender);
	m_pSceneRender->init(hWnd);
	return JZ_SUCCESS;
}

// 初始化图像处理接口和参数
JZ_RESULT JZUIEngine::_InitImageProcessSDK()
{
	// 初始化基本图像处理接口和参数
	m_pBaseImageProcess = NULL;
	g_JZBaseImageProcessAPI->pfnGetInterface(&m_pBaseImageProcess);
	JZSmoothParam* pSmoothParam = new JZSmoothParam();
	m_mapBaseProcessParam.insert(pair<JZ_BASEPROCESS_TYPE, JZCommonParam*>(JZ_BASEPROCESS_SMOOTH, pSmoothParam));
	JZMorphologyParam* pMorphologyParam = new JZMorphologyParam();
	m_mapBaseProcessParam.insert(pair<JZ_BASEPROCESS_TYPE, JZCommonParam*>(JZ_BASEPROCESS_MORPHOLOGY, pMorphologyParam));
	JZHistogramParam* pHistogramParam = new JZHistogramParam();
	m_mapBaseProcessParam.insert(pair<JZ_BASEPROCESS_TYPE, JZCommonParam*>(JZ_BASEPROCESS_HISTOGRAM, pHistogramParam));
	// 初始化车牌识别接口和参数
	m_pImagePlateRecog = NULL;
	g_JZImagePlateRecogAPI->pfnGetInterface(&m_pImagePlateRecog);
	m_pPlateRecogParam = new JZPlateRecogParam();

	// 初始化图像数据
	static JZImageBuf src = { 0 };
	static JZImageBuf des = { 0 };
	m_imageProcessData.pSrcImage = &src;
	m_imageProcessData.pDesImage = &des;

	return JZ_SUCCESS;
}

// 释放图形渲染接口
JZ_RESULT JZUIEngine::_ReleaseRenderSDK()
{
	if (NULL != m_pSceneRender)
	{
		g_JZBaseRenderAPI->pfnReleaseSceneInterface(m_pSceneRender);
		m_pSceneRender = NULL;
	}

	return JZ_SUCCESS;
}

// 释放图像处理基类接口和参数
JZ_RESULT JZUIEngine::_ReleaseImageProcessSDK()
{
	// 防止用户忘记释放图像数据
	_ReleaseImageData();		

	// 释放图像处理SDK接口
	if (NULL != m_pBaseImageProcess)
	{
		
		g_JZBaseImageProcessAPI->pfnReleaseInterface(m_pBaseImageProcess);// 释放基本图像处理接口
		m_pBaseImageProcess = NULL;
	}

	if (NULL != m_pImagePlateRecog)
	{
		g_JZImagePlateRecogAPI->pfnReleaseInterface(m_pImagePlateRecog);// 释放车牌识别接口
		m_pImagePlateRecog = NULL;
	}

	// 释放图像处理参数
	for (map<JZ_BASEPROCESS_TYPE, JZCommonParam*>::iterator it = m_mapBaseProcessParam.begin();
	it != m_mapBaseProcessParam.end();) // 释放基本图像处理参数
	{
		if (NULL != it->second)
		{
			delete it->second;
			it->second = NULL;
			it = m_mapBaseProcessParam.erase(it);
		}
		else
		{
			it++;
		}
	}

	if (NULL != m_pPlateRecogParam)
	{
		delete m_pPlateRecogParam;
		m_pPlateRecogParam = NULL;
	}

	return JZ_SUCCESS;
}

// 释放图像数据
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

	if (NULL != m_imageProcessData.pExtraData)
	{
		delete m_imageProcessData.pExtraData;
	}
	return JZ_SUCCESS;
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
