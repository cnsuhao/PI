#ifndef __IJZ_UIENGINE_H__
#define __IJZ_UIENGINE_H__
#include <JZCommonImageDef.h>
#include <Windows.h>

class IJZUIEngine
{
public:
	// 初始化UIEngine中成员
	virtual JZ_RESULT Init(HWND hWnd) = 0;
	// 释放UIEngine中成员
	virtual JZ_RESULT Release() = 0;
	// 设置要处理的图像数据
	virtual JZ_RESULT SetImageData(const char* filename) = 0;
	// 是否已经设置了要处理的图像
	virtual bool IsSetSrcImage() = 0;
	// 将处理后的图像数据保存为图片
	virtual JZ_RESULT SaveImageData(const char* filename) = 0;
	// 设置当前图像处理类型
	virtual JZ_RESULT SetCurProcessType(JZ_IMAGEPROC_TYPE curProcessType) = 0;
	// 设置图像处理的参数
	virtual JZ_RESULT SetProcessParam(JZCommonParam* param) = 0;
	// 根据图像处理类型进行图像处理
	virtual JZ_RESULT ProcessImage() = 0;
	// 渲染场景
	virtual JZ_RESULT Render() = 0;

};

typedef JZ_RESULT(*DefGetUIEngineInterface)(IJZUIEngine** ppAPI);
typedef JZ_RESULT(*DefReleaseUIEngineInterface)(IJZUIEngine*& pAPI);

struct JZUIEngineAPI
{
	DefGetUIEngineInterface pfnGetInterface;
	DefReleaseUIEngineInterface pfnReleaseInterface;
};
#endif // __IJZ_UIENGINE_H__

