#ifndef __IJZ_BASEIMAGEPROCESS_H__
#define __IJZ_BASEIMAGEPROCESS_H__

#include <Windows.h>
#include <tchar.h>
#include <JZCommonImageDef.h>

#ifdef BASEIMAGEPROCESS_EXPORTS
#define BASEIMAGEPROCESS_API __declspec(dllexport)
#else
#define BASEIMAGEPROCESS_API __declspec(dllimport)
#ifdef _DEBUG
#	pragma comment(lib, "JZBaseImageProcessd.lib")
#else
#	pragma comment(lib, "JZBaseImageProcess.lib")
#endif // _DEBUG


#endif

// 接口基类
class IJZBaseImageProcess
{
public:
	// 写图像
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
	// 处理图像
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
	// 释放图像数据
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage) = 0;
};

class BASEIMAGEPROCESS_API JZBaseImageProcess : public IJZBaseImageProcess
{
public:
	// 构造函数
	JZBaseImageProcess();
	// 析构函数
	~JZBaseImageProcess();
	// 写图像
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// 读图像，调用了ReadImage()函数，就要调用ReleaseImage()函数进行释放
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// 处理图像
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) { return JZ_SUCCESS; }
	// 释放图像数据
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage);

};

typedef JZ_RESULT(*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);

// 供外部调用的接口声明
struct JZBaseImageProcessAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};

#endif
