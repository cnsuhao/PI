#define BASEIMAGEPROCESS_EXPORTS
#include "JZBaseImageProcess.h"
#include <opencv2\opencv.hpp>
using namespace cv;

#ifdef _DEBUG
#	pragma comment(lib, "opencv_world320d.lib")
#else
#	pragma comment(lib, "opencv_world320.lib")
#endif // _DEBUG

JZBaseImageProcess::JZBaseImageProcess() {}
JZBaseImageProcess::~JZBaseImageProcess() {}

JZ_RESULT JZBaseImageProcess::ReadImage(IN const char* filename, OUT JZImageBuf* pImage)
{
	String strFilename(filename);
	Mat image = imread(filename); // OpenCV读图片时，默认像素格式BGR

	if (image.empty())
	{
		return JZ_FAILED;
	}

	pImage->width = image.cols;
	pImage->height = image.rows;
	pImage->pixel_fmt = JZ_PIXFMT_BGR;
	pImage->pitch = image.step[0];
	int iImageBytes = pImage->height * pImage->pitch;
	pImage->color = new byte[iImageBytes];
	memcpy_s(pImage->color, iImageBytes, image.data, iImageBytes);
	return JZ_SUCCESS;
}

JZ_RESULT JZBaseImageProcess::WriteImage(JZImageBuf* pImageBuf, const char* filename)
{
	if (NULL == pImageBuf || NULL == pImageBuf->color)
	{
		Mat image = Mat::zeros(Size(500, 500), CV_8UC3);
		imwrite(filename, image);
		return JZ_FAILED;
	}

	Mat image(Size(pImageBuf->width, pImageBuf->height), CV_8UC3);
	int iImageBytes = pImageBuf->height * pImageBuf->pitch;
	memcpy_s(image.data, iImageBytes, pImageBuf->color, iImageBytes);
	imwrite(filename, image);
	return JZ_SUCCESS;
}

JZ_RESULT JZBaseImageProcess::ReleaseImage(JZImageBuf* pImage)
{
	if (NULL != pImage && NULL != pImage->color)
	{
		delete[] pImage->color;
		pImage->color = NULL;
	}

	return JZ_SUCCESS;
}

JZ_RESULT JZBaseImageProcess::ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
{

	JZBaseProcessParam* pBaseProcessParam = (JZBaseProcessParam*)pParam;
	switch (pBaseProcessParam->baseProcessType)
	{
	case JZ_BASEPROCESS_MORPHOLOGY:
		_ImageMorphology(pImageProcessData, pParam);
		break;
	case JZ_BASEPROCESS_SMOOTH:
		_ImageSmooth(pImageProcessData, pParam);
		break;
	default:
		break;
	}
	return JZ_SUCCESS;
}

//////////////////////////////////// 内部调用的私有函数////////////////////////////////
JZ_RESULT JZBaseImageProcess::_ImageMorphology(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
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

JZ_RESULT JZBaseImageProcess::_ImageSmooth(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
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
	*ppAPI = new JZBaseImageProcess();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZBaseImageProcess*& pAPI)
{
	if (NULL != pAPI)
	{
		delete (JZBaseImageProcess*)pAPI;
		pAPI = NULL;
	}
	return JZ_SUCCESS;
}

JZBaseImageProcessAPI* g_pBaseImageProcessAPI;
extern "C" _declspec(dllexport) void* JZBIP_GetAPIStuPtr()
{
	static JZBaseImageProcessAPI temp = { 0 }; // 给静态变量分配的内存区域，只有在程序停止运行时，才会收回
	
	g_pBaseImageProcessAPI = &temp;
	g_pBaseImageProcessAPI->pfnGetInterface = GetInterface;
	g_pBaseImageProcessAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pBaseImageProcessAPI;
}