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
	Mat image = imread(filename); // OpenCV��ͼƬʱ��Ĭ�����ظ�ʽBGR

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

//////////////////////////////////// �ڲ����õ�˽�к���////////////////////////////////
JZ_RESULT JZBaseImageProcess::_ImageMorphology(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
{
	if (NULL == pImageProcessData->pSrcImage->color || NULL == pImageProcessData->pDesImage->color)
	{
		return JZ_FAILED;
	}

	// ��ȡԴͼ���Ŀ��ͼ��
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
	case 0:// ��ʴ
		cvMorphType = MORPH_ERODE;
		break;
	case 1:// ����
		cvMorphType = MORPH_DILATE;
		break;
	case 2:// �����㣺��ͨ�˲���ȥ����������
		cvMorphType = MORPH_OPEN;
		break;
	case 3:// �����㣺ͼ���ⲿ�˲���ȥ��ǰ������
		cvMorphType = MORPH_CLOSE;
		break;
	case 4:// ��̬ѧ�ݶȣ�����ͼ�͸�ʴͼ֮����Խ��ſ�ı�Եͻ������������������������ı�Ե����
		cvMorphType = MORPH_GRADIENT;
		break;
	case 5:/* ��ñ��ԭͼ���뿪������ͼ֮���ԭͼ�м�ȥ��������ͼ����΢С��Ʒ�Ƚ��й��ɵ�����£�����ʹ�ö�ñ������б�����ȡ��*/
		cvMorphType = MORPH_TOPHAT;
		break;
	case 6:// ��ñ:��������ͼ��ԭͼ��֮���ñ������Ч��ͼͻ���˱�ԭͼ������Χ���������������
		cvMorphType = MORPH_BLACKHAT;
		break;
	case 7:// ���л�����
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

	// ��ȡԴͼ���Ŀ��ͼ��
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
	case JZ_SMOOTH_GAUSSIAN:	// ��˹�˲�
		GaussianBlur(srcImage, desImage, Size(15, 15), 3, 3, BORDER_DEFAULT);
		break;
	case JZ_SMOOTH_MEAN:	// ��ֵ�˲�
		blur(srcImage, desImage, Size(15, 15), Point(-1, -1), BORDER_DEFAULT);
		break;
	case JZ_SMOOTH_MEDIAN: // ��ֵ�˲�
		medianBlur(srcImage, desImage, 15);
		break;
	case JZ_SMOOTH_BILATERAL: // ˫���˲�
		bilateralFilter(srcImage, desImage, 15, 50, 3, BORDER_DEFAULT);
		break;
	default:
		GaussianBlur(srcImage, desImage, Size(15, 15), 3, 3, BORDER_DEFAULT);
		break;
	}

	memcpy_s(pdes->color, iImageBytes, desImage.data, iImageBytes);

	return JZ_SUCCESS;
}

//////////////////////////////////////////�����Ľӿ�////////////////////////////////
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
	static JZBaseImageProcessAPI temp = { 0 }; // ����̬����������ڴ�����ֻ���ڳ���ֹͣ����ʱ���Ż��ջ�
	
	g_pBaseImageProcessAPI = &temp;
	g_pBaseImageProcessAPI->pfnGetInterface = GetInterface;
	g_pBaseImageProcessAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pBaseImageProcessAPI;
}