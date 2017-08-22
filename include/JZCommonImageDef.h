#ifndef __JZ_COMMONIMAGEDEF_H__
#define __JZ_COMMONIMAGEDEF_H__

// ���ؽ������
enum JZ_RESULT
{
	JZ_SUCCESS,
	JZ_FAILED,
	JZ_INVAILD_PARAM,
};

// ͼ�����һЩ�����Ĺ�������
struct JZCommonParam
{

};

// ���ظ�ʽ��Ŀǰ��֧�ִ�����ظ�ʽ����Ƭ���ظ�ʽ�Ժ���չ
enum JZ_PIXEL_FMT
{
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

};

#endif // !__JZ_COMMONIMAGEDEF_H__
