#ifndef __JZ_MESH_H__
#define __JZ_MESH_H__
#include <IJZBaseRender.h>

class JZMesh: public IJZMesh
{
public:
	// 构造函数
	JZMesh();
	// 析构函数
	~JZMesh();
	// 获取资源类型
	virtual JZResType GetResType();
	// 创建网格资源
	virtual JZ_RESULT Create(std::vector<Vertex> vertexSet, std::vector<unsigned int> indexSet, JZ_DRAW_UNIT drawUnit, JZ_DRAW_METHOD drawMethod);
	// 创建用于显示图像的网格资源
	virtual JZ_RESULT CreateQuadMesh();
	// 释放网格资源
	virtual JZ_RESULT Release();
	// 绘制网格
	virtual JZ_RESULT Draw();
private:
	unsigned int m_VAO;						// VAO
	unsigned int m_VBO;						// VBO
	unsigned int m_EBO;						// 顶点索引buffer VBO
	int m_vertexNums;						// 顶点个数
	int m_indexNums;						// 索引个数
	JZ_DRAW_UNIT m_drawUnit;				// 绘制单元类型，有点、线、三角形
	JZ_DRAW_METHOD m_drawMethod;			// 绘制方法分按顶点绘制、顶点索引绘制
};

#endif // !__JZ_MESH_H__

