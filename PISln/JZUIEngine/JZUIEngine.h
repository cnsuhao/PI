#ifndef __JZ_UIENGINE_H__
#define __JZ_UIENGINE_H__
#include <IJZBaseRender.h>
#include <IJZUIEngine.h>
#include <IJZBaseImageProcess.h>
#include <map>
using namespace std;

class JZUIEngine: public IJZUIEngine
{
public:
	// 构造函数
	JZUIEngine();
	// 析构函数
	~JZUIEngine();
	// 初始化UIEngine中成员
	virtual JZ_RESULT Init(HWND hWnd);
	// 释放UIEngine中成员
	virtual JZ_RESULT Release();
	// 初始化图形渲染接口
	virtual JZ_RESULT InitRenderSDK(HWND hWnd);
	// 释放图形渲染接口
	virtual JZ_RESULT ReleaseRenderSDK();
	// 初始化图像处理基类接口
	virtual JZ_RESULT InitBaseImageSDK();
	// 释放图像处理基类接口
	virtual JZ_RESULT ReleaseBaseImageSDK();
	// 初始化图像处理插件接口
	virtual JZ_RESULT InitImageProcessPlugin();
	// 释放图像处理插件接口
	virtual JZ_RESULT ReleaseImageProcessPlugin();
	// 设置要处理的图像数据
	virtual JZ_RESULT SetImageData(const char* filename);
	// 释放图像数据
	virtual JZ_RESULT ReleaseImageData();
	// 设置图像处理的参数
	virtual JZ_RESULT SetProcessParam(JZCommonParam param, JZ_IMAGEPROC_TYPE eImageProcType);
	// 根据图像处理类型进行图像处理
	virtual JZ_RESULT ProcessImage(JZ_IMAGEPROC_TYPE eImageProcType);
	// 渲染场景
	virtual JZ_RESULT Render();
	

private:
	void _ReleaseImageProcessAPI(JZ_IMAGEPROC_TYPE eImageProcType, IJZBaseImageProcess*& pBaseImageProcess);

private:
	IJZSceneRender*									m_pSceneRender;			// 图像渲染场景
	JZImageProcessData								m_imageProcessData;		// 用于图像处理的图像数据
	map<JZ_IMAGEPROC_TYPE, IJZBaseImageProcess*>	m_mapImageProcess;		// 存放不同图像处理方法的接口  <图像处理名字-图像处理接口>
	map<JZ_IMAGEPROC_TYPE, JZCommonParam*>			m_mapProcessParam;		// 存放不同图像处理方法的参数  <图像处理名字-图像处理参数>
	IJZBaseImageProcess*							m_pBaseImageProcess;	// 图像处理基类接口
};

#endif // __JZ_UIENGINE_H__
