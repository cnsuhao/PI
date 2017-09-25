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

//////////////////////////////////////////�����Ľӿ�////////////////////////////////
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