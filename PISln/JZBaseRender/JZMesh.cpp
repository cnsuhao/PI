#include "JZMesh.h"
#include <GL/glew.h>

#pragma comment(lib, "opengl32.lib")
#ifdef _DEBUG
#	ifdef _WIN64
#		pragma comment(lib, "glew64d.lib")
#	else
#		pragma comment(lib, "glew32d.lib")
#	endif
#else
#	ifdef _WIN64
#		pragma comment(lib, "glew64.lib")
#	else
#		pragma comment(lib, "glew32.lib")
#	endif
#endif // _DEBUG

JZMesh::JZMesh()
{
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_vertexNums = 0;
	m_indexNums = 0;
	m_drawMethod = JZ_DRAW_ARRAYS;
	m_drawUnit = JZ_DRAW_TRIANGLES;
}

JZMesh::~JZMesh()
{
	Release();
}

JZResType JZMesh::GetResType()
{
	return JZ_RES_MESH;
}

JZ_RESULT JZMesh::Create(std::vector<Vertex> vertexSet, std::vector<unsigned int> indexSet, JZ_DRAW_UNIT drawUnit, JZ_DRAW_METHOD drawMethod)
{
	if (vertexSet.empty())	// 没有顶点数据，直接返回
	{
		return JZ_FAILED;
	}

	if (JZ_DRAW_ELEMENTS == drawMethod && indexSet.empty())	// 如果是索引绘制却没有索引数据，直接返回
	{
		return JZ_FAILED;
	}

	m_vertexNums = vertexSet.size();
	m_indexNums = indexSet.size();
	m_drawUnit = drawUnit;
	m_drawMethod = drawMethod;

	GLenum eRet = 0;
	switch (m_drawMethod)
	{
	case JZ_DRAW_ARRAYS:
	{
		GLfloat* vertices = new GLfloat[vertexSet.size() * 8];
		for (unsigned int i = 0; i < vertexSet.size(); i++)
		{
			vertices[i * 8 + 0] = vertexSet[i].position.x;
			vertices[i * 8 + 1] = vertexSet[i].position.y;
			vertices[i * 8 + 2] = vertexSet[i].position.z;
			vertices[i * 8 + 3] = vertexSet[i].color.r;
			vertices[i * 8 + 4] = vertexSet[i].color.g;
			vertices[i * 8 + 5] = vertexSet[i].color.b;
			vertices[i * 8 + 6] = vertexSet[i].texture.x;
			vertices[i * 8 + 7] = vertexSet[i].texture.y;
		}
		
		// 生成VAO和VBO
		if (0 != m_VAO && 0 != m_VBO)
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
		}
		
		// VBO创建和传数据时，必须绑定在VAO上
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexSet.size() * 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		assert(0 == glGetError());
		break;
	}
		
	case JZ_DRAW_ELEMENTS:
	{
		GLfloat* vertices = new GLfloat[vertexSet.size() * 8];
		for (unsigned int i = 0; i < vertexSet.size(); i++)
		{
			vertices[i * 8 + 0] = vertexSet[i].position.x;
			vertices[i * 8 + 1] = vertexSet[i].position.y;
			vertices[i * 8 + 2] = vertexSet[i].position.z;
			vertices[i * 8 + 3] = vertexSet[i].color.r;
			vertices[i * 8 + 4] = vertexSet[i].color.g;
			vertices[i * 8 + 5] = vertexSet[i].color.b;
			vertices[i * 8 + 6] = vertexSet[i].texture.x;
			vertices[i * 8 + 7] = vertexSet[i].texture.y;
		}

		GLuint* verticesIndex = new GLuint[indexSet.size()];
		for (unsigned int i = 0; i < indexSet.size(); i++)
		{
			verticesIndex[i] = indexSet[i];
		}

		// 生成VAO、VBO和EBO
		if (0 == m_VAO && 0 == m_VBO && 0 == m_EBO)
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);
		}
		
		// VBO创建和传数据时，必须绑定在VAO上
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertexSet.size() * 8 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		
		glBindVertexArray(0);

		// EBO可以单独创建，传数据，只需要在绘制的时候绑定到VAO上即可
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSet.size() * sizeof(GLuint), verticesIndex, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		assert(0 == glGetError());

		delete[] vertices;
		delete[] verticesIndex;
		break;
	}
		
	default :
		break;
	}

	return JZ_SUCCESS;
}

JZ_RESULT JZMesh::CreateQuadMesh()
{
	GLfloat vertices[] =
	{
		//-----位置-----		-----颜色-----		---纹理坐标---
		1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 右上
		1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 右下
		-1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 左下
		-1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // 左上
	};

	std::vector<Vertex> vertexSet;
	for (int i = 0; i < 4; i++)
	{
		Vertex vertex;
		vertex.position.x = vertices[i * 8 + 0];
		vertex.position.y = vertices[i * 8 + 1];
		vertex.position.z = vertices[i * 8 + 2];
		vertex.color.r = vertices[i * 8 + 3];
		vertex.color.g = vertices[i * 8 + 4];
		vertex.color.b = vertices[i * 8 + 5];
		vertex.texture.x = vertices[i * 8 + 6];
		vertex.texture.y = vertices[i * 8 + 7];
		vertexSet.push_back(vertex);
	}

	std::vector<unsigned int> indexSet;
	indexSet.push_back(0);
	indexSet.push_back(1);
	indexSet.push_back(3);
	indexSet.push_back(1);
	indexSet.push_back(2);
	indexSet.push_back(3);

	return Create(vertexSet, indexSet, JZ_DRAW_TRIANGLES, JZ_DRAW_ELEMENTS);
}

JZ_RESULT JZMesh::CreateQuadMesh(float rectAspect, float texAspect)
{
	std::vector<Vertex> vertexSet;

	if (rectAspect < texAspect) // 当绘制区域的宽高比小于纹理宽高比时，也就是纹理的高度相对高于绘制区域时
	{
		float value = rectAspect / texAspect;
		GLfloat vertices[] =
		{
			//-----位置-----		-----颜色-----		---纹理坐标---
			1.0f,  value, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 右上
			1.0f, -value, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 右下
			-1.0f, -value, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 左下
			-1.0f,  value, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // 左上
		};

		for (int i = 0; i < 4; i++)
		{
			Vertex vertex;
			vertex.position.x = vertices[i * 8 + 0];
			vertex.position.y = vertices[i * 8 + 1];
			vertex.position.z = vertices[i * 8 + 2];
			vertex.color.r = vertices[i * 8 + 3];
			vertex.color.g = vertices[i * 8 + 4];
			vertex.color.b = vertices[i * 8 + 5];
			vertex.texture.x = vertices[i * 8 + 6];
			vertex.texture.y = vertices[i * 8 + 7];
			vertexSet.push_back(vertex);
		}
	}
	else  // 当绘制区域的宽高比大于纹理宽高比时，也就是纹理的宽度相对宽于绘制区域时
	{
		float value = texAspect / rectAspect;
		GLfloat vertices[] =
		{
			//-----位置-----		-----颜色-----		---纹理坐标---
			value,  1.0, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // 右上
			value, -1.0, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // 右下
			-value, -1.0, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 1.0f,   // 左下
			-value,  1.0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 0.0f    // 左上
		};

		for (int i = 0; i < 4; i++)
		{
			Vertex vertex;
			vertex.position.x = vertices[i * 8 + 0];
			vertex.position.y = vertices[i * 8 + 1];
			vertex.position.z = vertices[i * 8 + 2];
			vertex.color.r = vertices[i * 8 + 3];
			vertex.color.g = vertices[i * 8 + 4];
			vertex.color.b = vertices[i * 8 + 5];
			vertex.texture.x = vertices[i * 8 + 6];
			vertex.texture.y = vertices[i * 8 + 7];
			vertexSet.push_back(vertex);
		}
	}
	
	std::vector<unsigned int> indexSet;
	indexSet.push_back(0);
	indexSet.push_back(1);
	indexSet.push_back(3);
	indexSet.push_back(1);
	indexSet.push_back(2);
	indexSet.push_back(3);

	return Create(vertexSet, indexSet, JZ_DRAW_TRIANGLES, JZ_DRAW_ELEMENTS);
}

JZ_RESULT JZMesh::Release()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	assert(0 == glGetError());
	// 让顶点属性(顶点坐标、颜色、纹理坐标)失效
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	assert(0 == glGetError());
	// 释放VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_VBO);
	assert(0 == glGetError());
	// 释放EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_EBO);
	assert(0 == glGetError());
	// 释放VAO
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &m_VAO);
	assert(0 == glGetError());

	return JZ_SUCCESS;
}

JZ_RESULT JZMesh::Draw()
{
	unsigned int drawUnit = 0;
	switch (m_drawUnit)
	{
	case JZ_DRAW_POINTS:
		drawUnit = GL_POINTS;
		break;
	case JZ_DRAW_LINES:
		drawUnit = GL_LINES;
		break;
	case JZ_DRAW_TRIANGLES:
		drawUnit = GL_TRIANGLES;
		break;
	default:
		break;
	}

	switch (m_drawMethod)
	{
	case JZ_DRAW_ARRAYS:
		// 绘制
		glBindVertexArray(m_VAO);
		// VBO创建和传数据时，必须绑定在VAO上
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(6 * sizeof(GLfloat)));
		glDrawArrays(drawUnit, 0, m_vertexNums);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		assert(0 == glGetError());
		break;
	case JZ_DRAW_ELEMENTS:
		// 绘制
		glBindVertexArray(m_VAO);
		// VBO创建和传数据时，必须绑定在VAO上
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (const void*)(6 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// EBO可以单独创建，传数据，只需要在绘制的时候绑定到VAO上即可
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glDrawElements(drawUnit, m_indexNums, GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);

		assert(0 == glGetError());
		break;
	default:
		break;
	}
	
	return JZ_SUCCESS;
}