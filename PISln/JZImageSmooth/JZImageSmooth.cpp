#include "JZImageSmooth.h"
#include <opencv2\opencv.hpp>
using namespace cv;

#ifdef _DEBUG
#	pragma comment(lib, "opencv_world320d.lib")
#else
#	pragma comment(lib, "opencv_world320.lib")
#endif // _DEBUG

JZ_RESULT JZImageSmooth::ImageSmooth(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param)
{

	return JZ_SUCCESS;
}

JZ_RESULT JZImageSmooth::ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
{
	// 高斯滤波
	JZImageBuf* psrc = pImageProcessData->pSrcImage;
	JZImageBuf* pdes = pImageProcessData->pDesImage;
	Mat srcImage(Size(psrc->width, psrc->height), CV_8UC3);
	int iImageBytes = psrc->height * psrc->pitch;
	memcpy_s(srcImage.data, iImageBytes, psrc->color, iImageBytes);

	Mat desImage;
	GaussianBlur(srcImage, desImage, Size(15, 15), 3, 3, BORDER_DEFAULT);
	memcpy_s(pdes->color, iImageBytes, desImage.data, iImageBytes);

	return JZ_SUCCESS;
}

//////////////////////////////////////////导出的接口////////////////////////////////
JZ_RESULT GetInterface(IJZBaseImageProcess** ppAPI)
{
	*ppAPI = new JZImageSmooth();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZBaseImageProcess* pAPI)
{
	if (NULL != pAPI)
	{
		delete (JZImageSmooth*)pAPI;
		pAPI = NULL;
	}

	return JZ_SUCCESS;
}


JZImageSmoothAPI* g_pImageSmoothAPI;
extern "C" _declspec(dllexport) void* JZIS_GetAPIStuPtr()
{
	static JZImageSmoothAPI temp = { 0 };
	g_pImageSmoothAPI = &temp;
	g_pImageSmoothAPI->pfnGetInterface = GetInterface;
	g_pImageSmoothAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pImageSmoothAPI;
}