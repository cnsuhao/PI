#include "IJZBaseImageProcess.h"

#include <opencv2\opencv.hpp>
using namespace cv;

#ifdef _DEBUG
#	pragma comment(lib, "opencv_world320d.lib")
#else
#	pragma comment(lib, "opencv_world320.lib")
#endif // _DEBUG

IJZBaseImageProcess::IJZBaseImageProcess()
{
}

IJZBaseImageProcess::~IJZBaseImageProcess()
{
}

JZ_RESULT IJZBaseImageProcess::ReadImage(const char* filename, void* pImage)
{
	/*Mat image = *(Mat*)pImage;
	String strFilename(filename);
	image = imread(filename);
	if (image.empty())
	{
		return JZ_FAILED;
	}*/
	return JZ_OK;
}

class JZImageBlur: public IJZBaseImageProcess
{
public:
	JZImageBlur(){}
	~JZImageBlur(){}
	JZ_RESULT ProcessImage(void* psrc, void*  pdes, JZCommonParam* param);
private:

};

JZ_RESULT JZImageBlur::ProcessImage(void* psrc, void*  pdes, JZCommonParam* param) 
{
	return JZ_OK; 
}

JZ_RESULT GetInterface(IJZBaseImageProcess** ppAPI) 
{
	*ppAPI = new JZImageBlur();
	return JZ_OK;
}

JZ_RESULT ReleaseInterface(IJZBaseImageProcess* pAPI)
{
	if (NULL != pAPI)
	{
		delete pAPI;
		pAPI = NULL;
	}
	return JZ_OK;
}

JZBaseImageProcessAPI* g_pAPI;
extern "C" _declspec(dllexport) void* JZBIP_GetAPIStuPtr()
{
	JZBaseImageProcessAPI temp = { 0 };
	g_pAPI = &temp;
	g_pAPI->pfnGetInterface = GetInterface;
	g_pAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pAPI;
}