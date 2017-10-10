#ifndef __JZ_COMMONIMAGEDEF_H__
#define __JZ_COMMONIMAGEDEF_H__
#include <vector>
#include <string>
#include <JZCommonDef.h>

// ���ظ�ʽ��Ŀǰ��֧�ִ�����ظ�ʽ����Ƭ���ظ�ʽ�Ժ���չ
enum JZ_PIXEL_FMT
{
	JZ_PIXFMT_BGR,
	JZ_PIXFMT_RGB,
	JZ_PIXFMT_BGRA,
	JZ_PIXFMT_RGBA,
	JZ_PIXFMT_YUVA,
};

struct JZImageBuf
{
	unsigned char*		color;			// ��ɫͨ��
	int					pitch;			// ��ɫͨ��һ�е��ֽ���
	unsigned char*		color_U;		// U ��Ƭ����
	int					u_Pitch;		// U ��Ƭ����һ�е��ֽ���
	unsigned char*		color_V;		// V ��Ƭ����
	int					v_Pitch;		// V ��Ƭ����һ�е��ֽ���
	unsigned char*		alpha;			// A ͨ�����������ظ�ʽδ�ã��粻�� A �����޷���� A ͨ����
	int					a_Pitch;		// A ͨ��һ�е��ֽ���
	JZ_PIXEL_FMT		pixel_fmt;		// ���ظ�ʽ
	int					width;			// ͼ����
	int					height;			// ͼ��߶�

};

// ͼ�������洢������
struct JZImageProcessExtraData
{
	std::vector<std::string>	vecPlateStrs;		// ���ڴ洢ʶ����ĳ����ַ���
};

// ͼ��������(Դ)buffer�����(Ŀ��)buffer
struct JZImageProcessData
{
	JZImageBuf*					pSrcImage;			// Ҫ����ͼ�����buffer
	JZImageBuf*					pDesImage;			// ���ڴ洢ͼ������buffer
	std::vector<std::string>	vecPlateStrs;		// ���ڴ洢ʶ����ĳ����ַ���
	JZImageProcessExtraData*	pExtraData;			// �洢��������
};

// ͼ��������
enum JZ_IMAGEPROC_TYPE
{
	JZ_IMAGE_UNKNOWN = -1,
	JZ_IMAGE_BASEPROCESS,
	JZ_IMAGE_PLATERECOG,
	
};

// ͼ�����һЩ�����Ĺ�������
struct JZCommonParam
{
	JZ_IMAGEPROC_TYPE processType;
	JZCommonParam()
	{
		processType = JZ_IMAGE_UNKNOWN;
	}
};

// ����ͼ��������
enum JZ_BASEPROCESS_TYPE
{
	JZ_BASEPROCESS_UNKNOWN,
	JZ_BASEPROCESS_SMOOTH,		// ͼ��ƽ��
	JZ_BASEPROCESS_MORPHOLOGY,	// ͼ����̬ѧ
};

// ����ͼ�������
struct JZBaseProcessParam : public JZCommonParam
{
	JZ_BASEPROCESS_TYPE baseProcessType;
	JZBaseProcessParam()
	{
		processType = JZ_IMAGE_BASEPROCESS;
		baseProcessType = JZ_BASEPROCESS_UNKNOWN;
	}
};

// ͼ��ƽ����������
enum JZ_SMOOTH_TYPE
{
	JZ_SMOOTH_GAUSSIAN = 0,	// ��˹�˲�
	JZ_SMOOTH_MEAN,	// ��ֵ�˲�
	JZ_SMOOTH_MEDIAN, // ��ֵ�˲�
	JZ_SMOOTH_BILATERAL, // ˫���˲�
};

struct JZSmoothParam : public JZBaseProcessParam
{
	JZ_SMOOTH_TYPE smoothType;
	JZSmoothParam()
	{
		baseProcessType = JZ_BASEPROCESS_SMOOTH;
		smoothType = JZ_SMOOTH_GAUSSIAN;
	}
};

// ͼ����̬ѧ�����������
enum JZ_MORPHOLOGY_TYPE
{
	JZ_MORPHOLOGY_ERODE, // ��ʴ
	JZ_MORPHOLOGY_DILATE, // ����
	JZ_MORPHOLOGY_OPEN, // ������
	JZ_MORPHOLOGY_CLOSE, // ������
	JZ_MORPHOLOGY_GRADIENT, // ��̬ѧ�ݶ�
	JZ_MORPHOLOGY_TOPHAT, // ��ñ
	JZ_MORPHOLOGY_BLACKHAT, // ��ñ
	JZ_MORPHOLOGY_HITMISS, // ���л�����

};

// ��̬ѧ�������
struct JZMorphologyParam: public JZBaseProcessParam
{
	JZ_MORPHOLOGY_TYPE morphologyType; // ��̬ѧ����
	int elementSize;  // Ԫ�ش�СΪ: 2 * elementSize + 1
	JZMorphologyParam()
	{
		baseProcessType = JZ_BASEPROCESS_MORPHOLOGY;
		morphologyType = JZ_MORPHOLOGY_ERODE;
		elementSize = 1;
	}
};

// ����ʶ�����
struct JZPlateRecogParam : public JZCommonParam
{
	JZPlateRecogParam()
	{
		processType = JZ_IMAGE_PLATERECOG;
	}
};

#endif // !__JZ_COMMONIMAGEDEF_H__
