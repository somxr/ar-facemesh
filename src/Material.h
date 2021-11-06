#pragma once

#include <Gl/glew.h> //bc we will attaching to the uniform variables in the shader



class Material
{
	public:
		Material();
		Material( GLfloat sIntensity, GLfloat shine);

		void UseMaterial(GLuint specularIntensityLocation, GLuint shininessLocation);

		~Material();



private:
	GLfloat specularIntensity; //how much specular light there should be (the bright reflection spot)
	GLfloat shininess; //sometimes called specularPower, how smooth the surface will look


};

