#pragma once
#include <GL/glew.h>


class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat * vertices, unsigned int *indices, unsigned int numOfVertices, unsigned int	numOfIndices);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount; //GLsizei is just an integer but OpenGL uses this to represent the size of stuff such as arrays

};

