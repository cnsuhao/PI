#include "IJZBaseImageProcess.h"



class JZImageBlur: public IJZBaseImageProcess
{
public:
	JZImageBlur(){}
	~JZImageBlur(){}
	JZ_RESULT ProcessImage(/*Mat* src, Mat*  des, JZCommonParam* param*/);
private:

};

JZ_RESULT JZImageBlur::ProcessImage(/*Mat* src, Mat*  des, JZCommonParam* param*/) { return JZ_OK; }

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