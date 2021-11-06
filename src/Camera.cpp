#include "Camera.h"

Camera::Camera()
{}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp; //we set the world up to point at the "sky" right at the beginning, the camera's up will be calculated and change later
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;
	

	update();

}

glm::mat4 Camera::calculateViewMatrix()
{
	//calculates a matrix which "looks at" something with our given values
	return glm::lookAt(position, position+front, up);
}

glm::vec3 Camera::getCameraPosition()
{
	return position;
}

void Camera::update()
{
	//calculate the front based on the yaw and pitch (see theory video)
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	//the front y axis is only dependent on where you're looking up or down
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	//normalize vector (set the magnitude to 1, we only care about the vector's direction not magnitude)
	front = glm::normalize(front);

	//find the right vector based on our front (which we've just calculated) and the worldUp
	right = glm::normalize(glm::cross(front, worldUp));
	//find our camera's up using cross product of our right vector and up vector
	up = glm::normalize(glm::cross(right, front));

}
 
void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;

	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange)
{
	xChange *= turnSpeed;
	yChange *= turnSpeed;

	yaw += xChange;
	pitch += yChange;

	//limit the head rotation up and down (pitch) because rotating the head all the way up and back or down and behind causes problems
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	update();
}

Camera::~Camera()
{

}