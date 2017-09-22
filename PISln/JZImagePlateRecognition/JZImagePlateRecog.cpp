#include "JZImagePlateRecog.h"
#include <opencv2\opencv.hpp>
using namespace cv;
#include <core/plate_recognize.h>
using namespace easypr;

#ifdef _DEBUG
#	pragma comment(lib, "opencv_world320d.lib")
#	pragma comment(lib, "libeasyprd.lib")
#else
#	pragma comment(lib, "opencv_world320.lib")
#	pragma comment(lib, "libeasypr.lib")
#endif // _DEBUG

JZ_RESULT JZImagePlateRecog::ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam)
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
	Mat desImage = srcImage.clone();

	JZPlateRecogParam* pPlateRecogParam = (JZPlateRecogParam*)pParam;

	CPlateRecognize pr;
	pr.setLifemode(true);
	pr.setDebug(false);
	pr.setMaxPlates(1);
	pr.setDetectType(easypr::PR_DETECT_CMSER);

	//vector<string> plateVec;
	vector<CPlate> plateVec;

	int result = pr.plateRecognize(srcImage, plateVec);
	//int result = pr.plateRecognizeAsText(src, plateVec);
	if (result == 0)
	{
		size_t num = plateVec.size();
		for (size_t j = 0; j < num; j++)
		{
			String plateStr = plateVec[j].getPlateStr();
			pImageProcessData->pExtraData->vecPlateStrs.push_back(plateStr);
			utils::drawRect(desImage, plateVec[j].getPlatePos(), Scalar(0, 200, 255));
		}
	}

	if (result != 0) cout << "result:" << result << endl;

	memcpy_s(pdes->color, iImageBytes, desImage.data, iImageBytes);

	return JZ_SUCCESS;
}

//////////////////////////////////////////导出的接口////////////////////////////////
JZ_RESULT GetInterface(IJZBaseImageProcess** ppAPI)
{
	*ppAPI = new JZImagePlateRecog();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZBaseImageProcess*& pAPI)
{
	if (NULL != pAPI)
	{
		delete (JZImagePlateRecog*)pAPI;
		pAPI = NULL;
	}

	return JZ_SUCCESS;
}


JZImagePlateRecogAPI* g_pImagePlateRecogAPI;
extern "C" _declspec(dllexport) void* JZIPR_GetAPIStuPtr()
{
	static JZImagePlateRecogAPI temp = { 0 };
	g_pImagePlateRecogAPI = &temp;
	g_pImagePlateRecogAPI->pfnGetInterface = GetInterface;
	g_pImagePlateRecogAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pImagePlateRecogAPI;
}