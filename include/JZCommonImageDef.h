#ifndef __JZ_COMMONIMAGEDEF_H__
#define __JZ_COMMONIMAGEDEF_H__

// ���ؽ������
enum JZ_RESULT
{
	JZ_UNKNOW = -1,
	JZ_SUCCESS,
	JZ_FAILED,
	JZ_INVAILD_PARAM,
};

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

// ͼ��������(Դ)buffer�����(Ŀ��)buffer
struct JZImageProcessData
{
	JZImageBuf*		pSrcImage;			// Ҫ����ͼ�����buffer
	JZImageBuf*		pDesImage;			// ���ڴ洢ͼ������buffer
};

// ͼ��������
enum JZ_IMAGEPROC_TYPE
{
	JZ_IMAGE_UNKNOW = -1,
	JZ_IMAGE_SMOOTH,
};

// ͼ�����һЩ�����Ĺ�������
struct JZCommonParam
{
	JZ_IMAGEPROC_TYPE processType;
	JZCommonParam()
	{
		processType = JZ_IMAGE_UNKNOW;
	}
};


enum JZ_SMOOTH_TYPE
{
	JZ_SMOOTH_GAUSSIAN = 0,
	JZ_SMOOTH_MEAN,
	JZ_SMOOTH_MEDIAN,
	JZ_SMOOTH_BILATERAL,
};
struct JZSmoothParam : public JZCommonParam
{
	JZ_SMOOTH_TYPE smoothType;
	JZSmoothParam()
	{
		processType = JZ_IMAGE_SMOOTH;
		smoothType = JZ_SMOOTH_GAUSSIAN;
	}
};

#endif // !__JZ_COMMONIMAGEDEF_H__
