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
	if (NULL == pImageProcessData->pSrcImage->color || NULL == pImageProcessData->pDesImage->color)
	{
		return JZ_FAILED;
	}

	// 获取源图像和目的图像
	JZImageBuf* psrc = pImageProcessData->pSrcImage;
	JZImageBuf* pdes = pImageProcessData->pDesImage;
	Mat srcImage(Size(psrc->width, psrc->height), CV_8UC3);
	int iImageBytes = psrc->height * psrc->pitch;
	memcpy_s(srcImage.data, iImageBytes, psrc->color, iImageBytes);
	Mat desImage;

	JZSmoothParam* pSmoothParam = (JZSmoothParam*)pParam;
	JZ_SMOOTH_TYPE smoothType = pSmoothParam->smoothType;

	
	switch (smoothType)
	{
	case JZ_SMOOTH_GAUSSIAN:	// 高斯滤波
		GaussianBlur(srcImage, desImage, Size(15, 15), 3, 3, BORDER_DEFAULT);
		break;
	case JZ_SMOOTH_MEAN:	// 均值滤波
		blur(srcImage, desImage, Size(15, 15), Point(-1, -1), BORDER_DEFAULT);
		break;
	case JZ_SMOOTH_MEDIAN: // 中值滤波
		medianBlur(srcImage, desImage, 15);
		break;
	case JZ_SMOOTH_BILATERAL: // 双边滤波
		bilateralFilter(srcImage, desImage, 15, 50, 3, BORDER_DEFAULT);
		break;
	default:
		GaussianBlur(srcImage, desImage, Size(15, 15), 3, 3, BORDER_DEFAULT);
		break;
	}
	
	memcpy_s(pdes->color, iImageBytes, desImage.data, iImageBytes);

	return JZ_SUCCESS;
}

//////////////////////////////////////////导出的接口////////////////////////////////
JZ_RESULT GetInterface(IJZBaseImageProcess** ppAPI)
{
	*ppAPI = new JZImageSmooth();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZBaseImageProcess*& pAPI)
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