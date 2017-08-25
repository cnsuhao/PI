#include "IJZBaseImageProcessProc.h"
int main()
{
	IJZBaseImageProcess* pAPI = NULL;
	JZ_RESULT res = g_JZBaseImageProcessAPI->pfnGetInterface(&pAPI);
	JZImageBuf src = { 0 };
	JZImageBuf des = { 0 };
	JZCommonParam param;
	res = pAPI->ReadImage("../../sys/images/test.jpg", NULL);
	res = g_JZBaseImageProcessAPI->pfnReleaseInterface(pAPI);

	system("pause");
	return 0;
}