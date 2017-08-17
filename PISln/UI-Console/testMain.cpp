#include "IJZBaseImageProcess.h"
int main()
{
	IJZBaseImageProcess* pAPI = NULL;
	JZ_RESULT res = g_JZBaseImageProcessAPI->pfnGetInterface(&pAPI);
	void* psrc = NULL, *pdes = NULL;
	JZCommonParam param;
	res = pAPI->ProcessImage(psrc, pdes, &param);
	res = g_JZBaseImageProcessAPI->pfnReleaseInterface(pAPI);
	return 0;
}