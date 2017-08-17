#include "IJZBaseImageProcess.h"

int main()
{
	IJZBaseImageProcess* pAPI = NULL;
	JZ_RESULT res = g_JZBaseImageProcessAPI->pfnGetInterface(&pAPI);
	res = pAPI->ProcessImage();
	res = g_JZBaseImageProcessAPI->pfnReleaseInterface(pAPI);
	return 0;
}