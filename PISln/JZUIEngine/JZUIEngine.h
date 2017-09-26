#ifndef __JZ_UIENGINE_H__
#define __JZ_UIENGINE_H__
#include <IJZBaseRender.h>
#include <IJZUIEngine.h>
#include <IJZBaseImageProcess.h>
#include <IJZImagePlateRecog.h>
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
	// 设置要处理的图像数据
	virtual JZ_RESULT SetImageData(const char* filename);
	// 是否已经设置了要处理的图像
	virtual bool IsSetSrcImage();
	// 将处理后的图像数据保存为图片
	virtual JZ_RESULT SaveImageData(const char* filename);
	// 设置图像处理的参数
	virtual JZ_RESULT SetProcessParam(JZCommonParam* param);
	// 根据图像处理类型进行图像处理
	virtual JZ_RESULT ProcessImage(JZ_IMAGEPROC_TYPE processType, JZ_BASEPROCESS_TYPE baseProcessType = JZ_BASEPROCESS_UNKNOWN);
	// 获取存储的额外图像处理结果数据
	virtual JZImageProcessExtraData* GetExtraData();
	// 渲染场景
	virtual JZ_RESULT Render();
	

private:
	// 初始化图形渲染接口
	JZ_RESULT _InitRenderSDK(HWND hWnd);
	// 初始化图像处理接口和参数
	JZ_RESULT _InitImageProcessSDK();
	// 释放图形渲染接口
	JZ_RESULT _ReleaseRenderSDK();
	// 释放图像处理基类接口和参数
	JZ_RESULT _ReleaseImageProcessSDK();
	// 释放图像数据
	JZ_RESULT _ReleaseImageData();
private:
	IJZSceneRender*									m_pSceneRender;			// 图像渲染场景
	JZImageProcessData								m_imageProcessData;		// 用于图像处理的图像数据
	IJZBaseImageProcess*							m_pBaseImageProcess;	// 基本图像处理接口
	map<JZ_BASEPROCESS_TYPE, JZCommonParam*>		m_mapBaseProcessParam;	// 基本图像处理参数
	IJZImagePlateRecog*								m_pImagePlateRecog;		// 车牌识别接口
	JZCommonParam*									m_pPlateRecogParam;		// 车牌识别参数
	bool											m_bIsInit;				// 标志当前引擎是否已经初始化了
};

#endif // __JZ_UIENGINE_H__
