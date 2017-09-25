#include "JZUIEngine.h"
#include <IJZBaseRenderProc.h>
#include <IJZBaseImageProcessProc.h>
#include <IJZImageSmoothProc.h>
#include <IJZImageMorphologyProc.h>
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

JZ_RESULT JZUIEngine::Init(HWND hWnd)
{
	JZ_RESULT res = JZ_UNKNOW;
	res = _InitRenderSDK(hWnd);
	res = _InitBaseImageSDK();
	res = _InitImageProcessPlugin();
	m_bIsInit = true;
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
	if (!m_bIsInit)
	{
		return JZ_FAILED;
	}

	_ReleaseImageData();// ����ǰ�����ͷ�û���ͷŵ�ͼ������

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

// ��������ͼ�����ݱ���ΪͼƬ
JZ_RESULT JZUIEngine::SaveImageData(const char* filename)
{
	JZ_RESULT res = m_pBaseImageProcess->WriteImage(m_imageProcessData.pDesImage, filename);
	return res;
}

JZ_RESULT JZUIEngine::SetProcessParam(JZCommonParam* pParam)
{
	switch (pParam->processType)
	{
	case JZ_IMAGE_BASEPROCESS:
	{
		JZBaseProcessParam* pBaseProcessParam = (JZBaseProcessParam*)pParam;
		switch (pBaseProcessParam->baseProcessType)
		{
		case JZ_BASEPROCESS_SMOOTH:
		{
			JZSmoothParam* pSmoothParam = (JZSmoothParam*)pParam;
			((JZSmoothParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_SMOOTH])->smoothType = pSmoothParam->smoothType;
			break;
		}
		case JZ_BASEPROCESS_MORPHOLOGY:
		{
			JZMorphologyParam* pMorphologyParam = new JZMorphologyParam();
			((JZMorphologyParam*)m_mapBaseProcessParam[JZ_BASEPROCESS_MORPHOLOGY])->morphologyType = pMorphologyParam->morphologyType;
			break;
		}
		default:
			break;
		}
		break;
	}
	case JZ_IMAGE_PLATERECOG:
	{
		break;
	}
	default:
		break;
	}
	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::ProcessImage(JZ_IMAGEPROC_TYPE processType, JZ_BASEPROCESS_TYPE baseProcessType/* = JZ_BASEPROCESS_UNKNOWN*/)
{
	if (JZ_IMAGE_UNKNOWN == processType)
	{
		return JZ_FAILED;
	}

	switch (processType)
	{
	case JZ_IMAGE_BASEPROCESS:
		m_pBaseImageProcess->ProcessImage(&m_imageProcessData, m_mapBaseProcessParam[baseProcessType]);
		break;
	case JZ_IMAGE_PLATERECOG:
		m_pImagePlateRecog->ProcessImage(&m_imageProcessData, m_mapProcessParam[processType]);
		break;
	default:
		break;
	}

	m_pSceneRender->SetRightImage(m_imageProcessData.pDesImage);
	return JZ_SUCCESS;
}

// ��ȡ�洢�Ķ���ͼ����������
JZImageProcessExtraData* JZUIEngine::GetExtraData()
{
	return m_imageProcessData.pExtraData;
}

JZ_RESULT JZUIEngine::Render()
{
	if (!m_bIsInit)
	{
		return JZ_FAILED;
	}
	m_pSceneRender->RenderScene();
	return JZ_SUCCESS;
}

//////////////////////////////////////////// ˽�к���//////////////////////////////////
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
	// ��ʼ������ͼ����ӿ�
	m_pBaseImageProcess = NULL;
	g_JZBaseImageProcessAPI->pfnGetInterface(&m_pBaseImageProcess);

	// ��ʼͼ���������������m_mapBaseProcessParam
	JZSmoothParam* smoothParam = new JZSmoothParam();
	m_mapBaseProcessParam.insert(pair<JZ_BASEPROCESS_TYPE, JZCommonParam*>(JZ_BASEPROCESS_SMOOTH, smoothParam));
	JZMorphologyParam* morphologyParam = new JZMorphologyParam();
	m_mapBaseProcessParam.insert(pair<JZ_BASEPROCESS_TYPE, JZCommonParam*>(JZ_BASEPROCESS_MORPHOLOGY, morphologyParam));

	// ��ʼ������ʶ��ӿ�
	m_pImagePlateRecog = NULL;
	g_JZImagePlateRecogAPI->pfnGetInterface(&m_pImagePlateRecog);
	// ��ʼ��ͼ������
	static JZImageBuf src = { 0 };
	static JZImageBuf des = { 0 };
	m_imageProcessData.pSrcImage = &src;
	m_imageProcessData.pDesImage = &des;

	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_ReleaseBaseImageSDK()
{
	// ��ֹ�û������ͷ�ͼ������
	_ReleaseImageData();		

	// �ͷŻ���ͼ����ӿ�
	if (NULL != m_pBaseImageProcess)
	{
		
		g_JZBaseImageProcessAPI->pfnReleaseInterface(m_pBaseImageProcess);
		m_pBaseImageProcess = NULL;
	}

	// �ͷų���ʶ��ӿ�
	if (NULL != m_pImagePlateRecog)
	{
		g_JZImagePlateRecogAPI->pfnReleaseInterface(m_pImagePlateRecog);
		m_pImagePlateRecog = NULL;
	}

	// �ͷ�ͼ�������
	for (map<JZ_BASEPROCESS_TYPE, JZCommonParam*>::iterator it = m_mapBaseProcessParam.begin();
	it != m_mapBaseProcessParam.end();)
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

	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_InitImageProcessPlugin()
{
	// ��ʼ��ͼ����ӿڣ�������m_mapImageProcess
	IJZImageProcessBase* tempBaseImageProcess = NULL;

	//// ͼ��ƽ��SDK�ӿ�
	//if (g_JZImageSmoothAPI)
	//{
	//	g_JZImageSmoothAPI->pfnGetInterface(&tempBaseImageProcess);	// ����ͼ��ƽ���ӿ�
	//	m_mapImageProcess.insert(pair<JZ_IMAGEPROC_TYPE, IJZImageProcessBase*>(JZ_IMAGE_SMOOTH, tempBaseImageProcess));

	//	// ��ʼͼ���������������m_mapProcessParam
	//	JZSmoothParam* param = new JZSmoothParam();
	//	m_mapProcessParam.insert(pair<JZ_IMAGEPROC_TYPE, JZCommonParam*>(JZ_IMAGE_SMOOTH, param));
	//}
	//
	//// ͼ����̬ѧ����SDK�ӿ�
	//if (g_JZImageMorphologyAPI)
	//{
	//	g_JZImageMorphologyAPI->pfnGetInterface(&tempBaseImageProcess); // ����ͼ����̬ѧ����ӿ�
	//	m_mapImageProcess.insert(pair<JZ_IMAGEPROC_TYPE, IJZImageProcessBase*>(JZ_IMAGE_MORPHOLOGY, tempBaseImageProcess));
	//	// ��ʼͼ���������������m_mapProcessParam
	//	JZMorphologyParam* param = new JZMorphologyParam();
	//	m_mapProcessParam.insert(pair<JZ_IMAGEPROC_TYPE, JZCommonParam*>(JZ_IMAGE_MORPHOLOGY, param));
	//}

	//// ����ʶ��SDK�ӿ�
	//if (g_JZImagePlateRecogAPI)
	//{
	//	g_JZImagePlateRecogAPI->pfnGetInterface(&tempBaseImageProcess); // ���ɳ���ʶ��ӿ�
	//	m_mapImageProcess.insert(pair<JZ_IMAGEPROC_TYPE, IJZImageProcessBase*>(JZ_IMAGE_PLATERECOG, tempBaseImageProcess));
	//	// ��ʼ����ʶ�������������m_mapProcessParam
	//	JZPlateRecogParam* param = new JZPlateRecogParam();
	//	m_mapProcessParam.insert(pair<JZ_IMAGEPROC_TYPE, JZCommonParam*>(JZ_IMAGE_PLATERECOG, param));
	//}

	return JZ_SUCCESS;
}

JZ_RESULT JZUIEngine::_ReleaseImageProcessPlugin()
{
	// �ͷ�ͼ����ӿ�
	for (map<JZ_IMAGEPROC_TYPE, IJZImageProcessBase*>::iterator it = m_mapImageProcess.begin();
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

	// �ͷ�ͼ�������
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

	if (NULL != m_imageProcessData.pExtraData)
	{
		delete m_imageProcessData.pExtraData;
	}
	return JZ_SUCCESS;
}

void JZUIEngine::_ReleaseImageProcessAPI(JZ_IMAGEPROC_TYPE eImageProcType, IJZImageProcessBase*& pBaseImageProcess)
{
	//switch (eImageProcType)
	//{
	//case JZ_IMAGE_SMOOTH:
	//	g_JZImageSmoothAPI->pfnReleaseInterface(pBaseImageProcess);
	//	pBaseImageProcess = NULL;
	//	break;
	//case JZ_IMAGE_MORPHOLOGY:
	//	g_JZImageMorphologyAPI->pfnReleaseInterface(pBaseImageProcess);
	//	pBaseImageProcess = NULL;
	//	break;
	//case JZ_IMAGE_PLATERECOG:
	//	g_JZImagePlateRecogAPI->pfnReleaseInterface(pBaseImageProcess);
	//	pBaseImageProcess = NULL;
	//	break;
	//default:
	//	break;
	//}
}

//////////////////////////////////////////// �ӿڵ���//////////////////////////////////
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
