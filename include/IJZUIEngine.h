#ifndef __IJZ_UIENGINE_H__
#define __IJZ_UIENGINE_H__
#include <JZCommonImageDef.h>
#include <Windows.h>

enum JZ_IMAGEPROC_TYPE
{
	JZ_IMAGE_UNKONW = -1,
	JZ_IMAGE_SMOOTH,
};

class IJZUIEngine
{
public:
	// 初始化UIEngine中成员
	virtual JZ_RESULT Init(HWND hWnd) = 0;
	// 释放UIEngine中成员
	virtual JZ_RESULT Release() = 0;
	// 初始化图形渲染接口
	virtual JZ_RESULT InitRenderSDK(HWND hWnd) = 0;
	// 释放图形渲染接口
	virtual JZ_RESULT ReleaseRenderSDK() = 0;
	// 初始化图像处理基类接口
	virtual JZ_RESULT InitBaseImageSDK() = 0;
	// 释放图像处理基类接口
	virtual JZ_RESULT ReleaseBaseImageSDK() = 0;
	// 初始化图像处理插件接口
	virtual JZ_RESULT InitImageProcessPlugin() = 0;
	// 释放图像处理插件接口
	virtual JZ_RESULT ReleaseImageProcessPlugin() = 0;
	// 设置要处理的图像数据
	virtual JZ_RESULT SetImageData(const char* filename) = 0;
	// 释放图像数据
	virtual JZ_RESULT ReleaseImageData() = 0;
	// 设置图像处理的参数
	virtual JZ_RESULT SetProcessParam(JZCommonParam param, JZ_IMAGEPROC_TYPE eImageProcType) = 0;
	// 根据图像处理类型进行图像处理
	virtual JZ_RESULT ProcessImage(JZ_IMAGEPROC_TYPE eImageProcType) = 0;
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

