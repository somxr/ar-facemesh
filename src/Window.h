#pragma once

#include<stdio.h>

#include<GL\glew.h>
#include<GLFW\glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	
	//checking if the X button at top right corner was clicked, or tried to exit application
	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }  

	//get the set of keys from the window 
	bool* getKeys() { return keys; }
	
	GLfloat getXChange();
	GLfloat getYchange();


	void swapBuffers(){ glfwSwapBuffers(mainWindow); }

	~Window(); 

private:
	GLFWwindow* mainWindow;

	GLint width, height;
	GLint bufferWidth, bufferHeight;

	//1024 is the range of ASCII keys. When you press a key, GLFW picks it up and we assign it a boolean at the index number to true, when done pressing it's false
	bool keys[1024]; 
	
	//last coordinates of mouse 
	GLfloat lastX;
	GLfloat lastY;
	//change in mouse coordinates
	GLfloat xChange;
	GLfloat yChange;
	//when we first start, the cursor could be anywhere. Check if this is the very first movement to initialise and avoid quick initial movement
	bool mouseFirstMoved;


	//create our callback functions such as the input 
	void createCallbacks();
	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode); //needs to be static bc GLFW will do a callback, and we need a static function to callback to. normal member function wouldn't work
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
};

