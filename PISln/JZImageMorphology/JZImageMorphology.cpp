#include "JZImageMorphology.h"
#include <opencv2\opencv.hpp>
using namespace cv;

#ifdef _DEBUG
#	pragma comment(lib, "opencv_world320d.lib")
#else
#	pragma comment(lib, "opencv_world320.lib")
#endif // _DEBUG

JZ_RESULT JZImageMorphology::ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
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

	JZMorphologyParam* pMorphologyParam = (JZMorphologyParam*)pParam;
	JZ_MORPHOLOGY_TYPE morphologyType = pMorphologyParam->morphologyType;
	int s = pMorphologyParam->elementSize * 2 + 1;
	Mat kernel = getStructuringElement(MORPH_RECT, Size(s, s), Point(-1, -1));

	MorphTypes cvMorphType;
	switch (morphologyType)
	{
	case 0:// 腐蚀
		cvMorphType = MORPH_ERODE;
		break;
	case 1:// 膨胀
		cvMorphType = MORPH_DILATE;
		break;
	case 2:// 开运算：低通滤波；去除背景噪声
		cvMorphType = MORPH_OPEN;
		break;
	case 3:// 闭运算：图形外部滤波；去除前景噪声
		cvMorphType = MORPH_CLOSE;
		break;
	case 4:// 形态学梯度：膨胀图和腐蚀图之差，可以将团块的边缘突出出来，可以用来保留物体的边缘轮廓
		cvMorphType = MORPH_GRADIENT;
		break;
	case 5:/* 顶帽：原图像与开运算结果图之差，从原图中减去开运算后的图，在微小物品比较有规律的情况下，可以使用顶帽运算进行背景提取。*/
		cvMorphType = MORPH_TOPHAT;
		break;
	case 6:// 黑帽:闭运算结果图与原图像之差，黑帽运算后的效果图突出了比原图轮廓周围的区域更暗的区域
		cvMorphType = MORPH_BLACKHAT;
		break;
	case 7:// 击中击不中
		cvMorphType = MORPH_HITMISS;
		break;
	default:
		break;
	}

	morphologyEx(srcImage, desImage, cvMorphType, kernel, Point(-1, -1));

	memcpy_s(pdes->color, iImageBytes, desImage.data, iImageBytes);

	return JZ_SUCCESS;
}

//////////////////////////////////////////导出的接口////////////////////////////////
JZ_RESULT GetInterface(IJZImageProcessBase** ppAPI)
{
	*ppAPI = new JZImageMorphology();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZImageProcessBase*& pAPI)
{
	if (NULL != pAPI)
	{
		delete (JZImageMorphology*)pAPI;
		pAPI = NULL;
	}

	return JZ_SUCCESS;
}


JZImageMorphologyAPI* g_pImageMorphologyAPI;
extern "C" _declspec(dllexport) void* JZIM_GetAPIStuPtr()
{
	static JZImageMorphologyAPI temp = { 0 };
	g_pImageMorphologyAPI = &temp;
	g_pImageMorphologyAPI->pfnGetInterface = GetInterface;
	g_pImageMorphologyAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pImageMorphologyAPI;
}