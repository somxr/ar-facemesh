#pragma once
#include <GL/glew.h>

#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

class Camera
{
public:
	
	Camera();
	Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed);
	
	void keyControl(bool* keys, GLfloat deltaTime);
	void mouseControl(GLfloat xChange, GLfloat yChange);

	glm::vec3 getCameraPosition();

	glm::mat4 calculateViewMatrix(); 

	~Camera();
	
private:
	glm::vec3 position; //camera position
	glm::vec3 front; //front direction 
	glm::vec3 up; //up vector
	glm::vec3 right; //right vector
	glm::vec3 worldUp; //rotating relative to the world's up 
	
	GLfloat yaw;
	GLfloat pitch;
	
	GLfloat moveSpeed;
	GLfloat turnSpeed;

	void update();
};

