#define BASEIMAGEPROCESS_EXPORTS
#include "JZBaseImageProcess.h"
#include <opencv2\opencv.hpp>
using namespace cv;
#include <cvui.h>

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
		return _ImageMorphology(pImageProcessData, pParam);
	case JZ_BASEPROCESS_SMOOTH:
		return _ImageSmooth(pImageProcessData, pParam);
	case JZ_BASEPROCESS_HISTOGRAM:
		return _ImageHistogram(pImageProcessData, pParam);
	default:
		break;
	}
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
	MorphShapes cvMorphShape;
	switch (pMorphologyParam->morphologyShape)
	{
	case 0:
		cvMorphShape = MORPH_RECT;
		break;
	case 1:
		cvMorphShape = MORPH_CROSS;
		break;
	case 2:
		cvMorphShape = MORPH_ELLIPSE;
		break;
	default:
		break;
	}
	if (0 == pMorphologyParam->width || 0 == pMorphologyParam->height)
	{
		return JZ_INVAILD_PARAM;
	}
	Size ksize = Size(pMorphologyParam->width, pMorphologyParam->height);
	Mat kernel = getStructuringElement(cvMorphShape, ksize, Point(-1, -1));

	MorphTypes cvMorphType;
	switch (pMorphologyParam->morphologyType)
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

JZ_RESULT JZBaseImageProcess::_ImageHistogram(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
{
	JZImageBuf* psrc = pImageProcessData->pSrcImage;
	JZImageBuf* pdes = pImageProcessData->pDesImage;
	Mat srcImage(Size(psrc->width, psrc->height), CV_8UC3);
	int iImageBytes = psrc->height * psrc->pitch;
	memcpy_s(srcImage.data, iImageBytes, psrc->color, iImageBytes);

	int resultWidth = 600;
	int resultHeight = 500;
	Mat resultHist(resultHeight, resultWidth, CV_8UC3, Scalar(77, 77, 51));
	//cv::namedWindow("ֱ��ͼ");

	//cvui::init("ֱ��ͼ", 20);
	//bool bBGRHist = cvui::button(resultHist, 200, 5, "BGRֱ��ͼ");
	//bool bHSVHist = cvui::button(resultHist, 400, 5, "HSVֱ��ͼ");

	//if (bHSVHist)
	//{
		Mat srcHSV;
		cvtColor(srcImage, srcHSV, COLOR_BGR2HSV);
		std::vector<Mat> vecHSV;
		split(srcHSV, vecHSV);

		//��ɫֱ��ͼ
		const int channelH[1] = { 0 };
		const int channelS[1] = { 0 };
		const int channelV[1] = { 0 };

		const int hHistSize[1] = { 256 };
		const int sHistSize[1] = { 256 };
		const int vHistSize[1] = { 256 };

		float rangeSingle[2] = { 0, 255 };
		const float* hRange[1] = { rangeSingle };
		const float* sRange[1] = { rangeSingle };
		const float* vRange[1] = { rangeSingle };

		Mat hHist, sHist, vHist;
		calcHist(&vecHSV[0], 1, channelH, Mat(), hHist, 1, hHistSize, hRange, true);
		calcHist(&vecHSV[1], 1, channelS, Mat(), sHist, 1, sHistSize, sRange, true);
		calcHist(&vecHSV[2], 1, channelV, Mat(), vHist, 1, vHistSize, vRange, true);

		Mat hNormHist, sNormHist, vNormHist;
		normalize(hHist, hNormHist, 0, resultHeight*0.2, NORM_MINMAX);
		normalize(sHist, sNormHist, 0, resultHeight*0.2, NORM_MINMAX);
		normalize(vHist, vNormHist, 0, resultHeight*0.2, NORM_MINMAX);


		int xBins = resultWidth / 255;
		for (int i = 1; i < hHistSize[0]; i++)
		{
			Point p1 = Point(5 + xBins*(i - 1), resultHeight - cvRound(vNormHist.at<float>(i - 1)) - resultHeight*0.1);
			Point p2 = Point(5 + xBins*(i), resultHeight - cvRound(vNormHist.at<float>(i)) - resultHeight*0.1);
			line(resultHist, p1, p2, Scalar(255, 0, 0), 1, LINE_AA);
		}
		for (int i = 1; i < sHistSize[0]; i++)
		{
			Point p1 = Point(5 + xBins*(i - 1), resultHeight - cvRound(sNormHist.at<float>(i - 1)) - resultHeight*0.4);
			Point p2 = Point(5 + xBins*(i), resultHeight - cvRound(sNormHist.at<float>(i)) - resultHeight*0.4);
			line(resultHist, p1, p2, Scalar(0, 255, 0), 1, LINE_AA);
		}
		for (int i = 1; i < vHistSize[0]; i++)
		{
			Point p1 = Point(5 + xBins*(i - 1), resultHeight - cvRound(hNormHist.at<float>(i - 1)) - resultHeight*0.7);
			Point p2 = Point(5 + xBins*(i), resultHeight - cvRound(hNormHist.at<float>(i)) - resultHeight*0.7);
			line(resultHist, p1, p2, Scalar(0, 0, 255), 1, LINE_AA);
		}

		cvui::text(resultHist, resultWidth*0.9, resultHeight*0.2, "H", 1.2, 0xff0000);
		cvui::text(resultHist, resultWidth*0.9, resultHeight*0.5, "S", 1.2, 0x00ff00);
		cvui::text(resultHist, resultWidth*0.9, resultHeight*0.8, "V", 1.2, 0x0000ff);
	/*}
	cvui::update();*/
	imshow("ֱ��ͼ", resultHist);

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