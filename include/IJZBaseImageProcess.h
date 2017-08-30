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

// �ӿڻ���
class IJZBaseImageProcess
{
public:
	// дͼ��
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param) = 0;
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage) = 0;
	// ����ͼ��
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) = 0;
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage) = 0;
};

class BASEIMAGEPROCESS_API JZBaseImageProcess : public IJZBaseImageProcess
{
public:
	// ���캯��
	JZBaseImageProcess();
	// ��������
	~JZBaseImageProcess();
	// дͼ��
	virtual JZ_RESULT WriteImage(JZImageBuf* psrc, JZImageBuf*  pdes, JZCommonParam* param);
	// ��ͼ�񣬵�����ReadImage()��������Ҫ����ReleaseImage()���������ͷ�
	virtual JZ_RESULT ReadImage(IN const char* filename, OUT JZImageBuf* pImage);
	// ����ͼ��
	virtual JZ_RESULT ProcessImage(JZImageProcessData* pImageProcessData, JZCommonParam* pParam) { return JZ_SUCCESS; }
	// �ͷ�ͼ������
	virtual JZ_RESULT ReleaseImage(JZImageBuf* pImage);

};

typedef JZ_RESULT(*DefGetInterface)(IJZBaseImageProcess** ppAPI);
typedef JZ_RESULT(*DefReleaseInterface)(IJZBaseImageProcess* pAPI);

// ���ⲿ���õĽӿ�����
struct JZBaseImageProcessAPI
{
	DefGetInterface pfnGetInterface;
	DefReleaseInterface pfnReleaseInterface;
};

#endif
