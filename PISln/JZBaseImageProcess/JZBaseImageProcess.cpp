#include "IJZBaseImageProcess.h"

#include <opencv2\opencv.hpp>
using namespace cv;

#ifdef _DEBUG
#	pragma comment(lib, "opencv_world320d.lib")
#else
#	pragma comment(lib, "opencv_world320.lib")
#endif // _DEBUG



class JZBaseImageProcess: public IJZBaseImageProcess
{
public:
	JZBaseImageProcess(){}
	~JZBaseImageProcess(){}
	JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  JZImageBuf, JZCommonParam* param);
	JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);

};

JZ_RESULT JZBaseImageProcess::WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param)
{
	return JZ_SUCCESS; 
}

JZ_RESULT JZBaseImageProcess::ReadImage(IN const char* filename, OUT JZImageBuf* pImage)
{
	String strFilename(filename);
	Mat image = imread(filename);
	if (image.empty())
	{
		return JZ_FAILED;
	}
	return JZ_SUCCESS;
}

JZ_RESULT GetInterface(IJZBaseImageProcess** ppAPI) 
{
	*ppAPI = new JZBaseImageProcess();
	return JZ_SUCCESS;
}

JZ_RESULT ReleaseInterface(IJZBaseImageProcess* pAPI)
{
	if (NULL != pAPI)
	{
		delete pAPI;
		pAPI = NULL;
	}
	return JZ_SUCCESS;
}

JZBaseImageProcessAPI* g_pAPI;
extern "C" _declspec(dllexport) void* JZBIP_GetAPIStuPtr()
{
	static JZBaseImageProcessAPI temp = { 0 }; // 给静态变量分配的内存区域，只有在程序停止运行时，才会收回
	
	g_pAPI = &temp;
	g_pAPI->pfnGetInterface = GetInterface;
	g_pAPI->pfnReleaseInterface = ReleaseInterface;
	return (void*)g_pAPI;
}