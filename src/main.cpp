#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <vector>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CommonValues.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Material.h"

const float toRadians = 3.14159265f / 180.0f; //This will do the conversion from degrees to radians by multiplying the number by it. 
												//Remember that radians scale between 0 -> 2PI as opposed to 0 -> 360

Window mainWindow;
std::vector<Mesh*> meshList; 
std::vector<Shader*>shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;

PointLight pointLights[MAX_POINT_LIGHTS];


GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f; 

// Vertex Shader 
static const char * vShader = "Shaders/shader.vert";  

//fragment shader
static const char * fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	//going thru indices, row by row that's why we're jumping i+=3 each loop
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		//extract the vertex represented by each of the indices we're working with
		//index0-2 gives us the current row we're working with and points to the x value in their respective vertices
		unsigned int index0 = indices[i] * vLength;
		unsigned int index1 = indices[i + 1] * vLength;
		unsigned int index2 = indices[i + 2] * vLength;
		glm::vec3 vector1(vertices[index1] - vertices[index0], vertices[index1 + 1] - vertices[index0 + 1], vertices[index1 + 2] - vertices[index0 + 2]);
		glm::vec3 vector2(vertices[index2] - vertices[index0], vertices[index2 + 1] - vertices[index0 + 1], vertices[index2 + 2] - vertices[index0 + 2]);
		glm::vec3 normal = glm::cross(vector1, vector2);
		normal = glm::normalize(normal);

		//add the normal offset so that the variables go from pointing to the 'x' value in each vertex to the first normal value for each vertex
		index0 += normalOffset;
		index1 += normalOffset;
		index2 += normalOffset;

		vertices[index0] += normal.x;
		vertices[index0 + 1] += normal.y;
		vertices[index0 + 2] += normal.z;

		vertices[index1] += normal.x;
		vertices[index1 + 1] += normal.y;
		vertices[index1 + 2] += normal.z;

		vertices[index2] += normal.x;
		vertices[index2 + 1] += normal.y;
		vertices[index2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);

		vertices[nOffset] = vec.x;
		vertices[nOffset + 1] = vec.y;
		vertices[nOffset + 2] = vec.z;
	}
}

void CreateObjects() 
{
	//set of indices that tell the graphics card which points to place in which order
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};
	
	GLfloat vertices[] = {
	//	  x		 y	   z		U		V	  normals: x    y     z
		-1.0f, -1.0f, -0.6f,	0.0f,  0.0f,		0.0f, 0.0f, 0.0f, 
		0.0f, -1.0f, 1.0f,		0.5f,  0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.6f,		1.0f, 0.0f,			0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,		0.5f, 1.0f,			0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1, 
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,  0.0f, 0.0f,    0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,   10.0f, 0.0f,   0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,   0.0f, 10.0f,   0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,	   10.0f, 10.0f,  0.0f, -1.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2 );

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);
}

void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main()
{
	mainWindow = Window(1366, 768); //originally 800 by 600
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3 (0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.2f); 

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTexture();

	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTexture();

	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTexture();

	shinyMaterial = Material(4.0f, 256); //The shine should be ideally power of 2. 32 is an avg value for shiny stuff usually
	dullMaterial = Material(0.3f, 4);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, //rgb
								0.5f, 0.5f,        //ambient/diffuse intensities
								0.0f, 0.0f, -1.0f);   //direction

	unsigned int pointLightCount = 0;

	pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
								0.0f, 1.0f, 
								0.0f, 0.0f, 0.0f,
								0.3f, 0.2f, 0.1f);

	pointLightCount++;

	pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
								0.0f, 1.0f,
								-4.0f, 2.0f, 0.0f,
								0.3f, 0.1f, 0.1f);

	pointLightCount++;

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,  
			uniformSpecularIntensity = 0, uniformShininess = 0; 

	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth()/mainWindow.getBufferHeight(), 0.1f, 100.0f);

	//loop until window closed
	while (!mainWindow.getShouldClose())
	{
		//store the current time
		GLfloat now = glfwGetTime(); //returns seconds 
		deltaTime = now - lastTime;
		lastTime = now;

		// Get and handle a user input event
		glfwPollEvents();

		//camera controls checking
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYchange());
		
		//Clear Window
		glClearColor(0.0f,0.0f,0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();
		uniformEyePosition = shaderList[0]->GetEyePositionLocation();
		uniformSpecularIntensity = shaderList[0]->GetSpecularIntensityLocation();
		uniformShininess = shaderList[0]->GetShininessLocation();

		shaderList[0]->SetDirectionalLight(&mainLight);
		shaderList[0]->SetPointLights(pointLights, pointLightCount);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//glm variable type matrix 4x4, automatically set to identity matrix (diagonal 1s across the matrix of 0s). We alter this to make out transform matrix
		glm::mat4 model(1.0f);

		//take the identity matrix and apply a translation to it. It will only alter the X value
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f)); 
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f,1.0f));

		//Assign the value to the shader itself. glUniform1f is saying we want to use uniform value which is a single (1) float (f) value 
		//glUniform1f(uniformXMove, triOffset); 
		
		//Uniform variable is a 4x4 matrix, and it will be holding floats
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
															//^^we need to use the value ptr bc the model itself is not directly in a raw format which would work with the shader

		brickTexture.UseTexture(); //from here on, anything drawn (rendered) will use this texture which we've loaded in
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();
		
		model = glm::mat4(1.0f); //go back to identity matrix to get rid of the translate and scale we've applied
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		//change texture before rendering next object
		dirtTexture.UseTexture(); 
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();


		model = glm::mat4(1.0f); 
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		plainTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();


		glUseProgram(0);

		mainWindow.swapBuffers();
		
	}

	return 0;

}






// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
