#ifndef __JZ_MESH_H__
#define __JZ_MESH_H__
#include <IJZBaseRender.h>

class JZMesh: public IJZMesh
{
public:
	// ���캯��
	JZMesh();
	// ��������
	~JZMesh();
	// ��ȡ��Դ����
	virtual JZResType GetResType();
	// ����������Դ
	virtual JZ_RESULT Create(std::vector<Vertex> vertexSet, std::vector<unsigned int> indexSet, JZ_DRAW_UNIT drawUnit, JZ_DRAW_METHOD drawMethod);
	// ����������ʾͼ���������Դ
	virtual JZ_RESULT CreateQuadMesh();
	// �ͷ�������Դ
	virtual JZ_RESULT Release();
	// ��������
	virtual JZ_RESULT Draw();
private:
	unsigned int m_VAO;						// VAO
	unsigned int m_VBO;						// VBO
	unsigned int m_EBO;						// ��������buffer VBO
	int m_vertexNums;						// �������
	int m_indexNums;						// ��������
	JZ_DRAW_UNIT m_drawUnit;				// ���Ƶ�Ԫ���ͣ��е㡢�ߡ�������
	JZ_DRAW_METHOD m_drawMethod;			// ���Ʒ����ְ�������ơ�������������
};

#endif // !__JZ_MESH_H__

