#include "Facemesh.h"

Facemesh::Facemesh() {

	for (int i = 0; i < (68 * 3); i++)
	{
		vertices[i] = 0;
	}

	VAO = 0;
	VBO_position = 0;
	VBO_TexCoord = 0;
	EBO = 0;
}

void Facemesh::updateVertex(int index, GLfloat x, GLfloat y, GLfloat z)
{
	vertices[index] = x;
	vertices[index + 1] = y;
	vertices[index + 2] = 1.0f;
}

void Facemesh::CreateMesh() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO_position);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(68*3*(sizeof(GLfloat))), NULL, GL_DYNAMIC_DRAW);
	
	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo

	// Texture Attribute
	glGenBuffers(1, &VBO_TexCoord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coord), texture_coord, GL_STATIC_DRAW);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL); 
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo

	//Index buffer
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0); // Unbind VAO

}

void Facemesh::Render()
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBindVertexArray(VAO);

	glPointSize(5.0f);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//glDrawArrays(GL_TRIANGLE_STRIP, 0, 68);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, 285, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_POINTS, 0, 68);

	glBindVertexArray(0);
}

void Facemesh::ClearMesh()
{
	if (EBO != 0)
	{
		glDeleteBuffers(1, &EBO);
		EBO = 0;
	}

	if (VBO_position != 0)
	{
		glDeleteBuffers(1, &VBO_position);
		VBO_position = 0;
	}

	if (VBO_TexCoord != 0)
	{
		glDeleteBuffers(1, &VBO_TexCoord);
		VBO_TexCoord = 0;
	}

	if (VAO != 0)
	{
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}
}

Facemesh::~Facemesh()
{

}