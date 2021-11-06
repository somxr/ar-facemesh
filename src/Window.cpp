#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;

	for (size_t i =0; i<1024; i++)
	{
		keys[i] = false;
	}

	xChange = 0.0f;
	yChange = 0.0f;

	mouseFirstMoved = true; 
}

Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

	xChange = 0.0f;
	yChange = 0.0f;

	mouseFirstMoved = true;
}

int Window::Initialise()
{
	//initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialization ERROR");
		glfwTerminate();
		return 1;
	}

	//Set up the GLFW window properties
	//OpenGL window version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//core profile = no backwards compatibility
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Allow forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create the window
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	if (!mainWindow)
	{
		printf("GLFW window creation failed");
		glfwTerminate();
		return 1;
	}

	// Get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set current context for GLEW to use
	glfwMakeContextCurrent(mainWindow);

	//Handle Key + Mouse Input//
	createCallbacks();
	//Set GLFW cursor to be disabled, no cursor enabled (hidden) and nothing clickable on the screen
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 

	//Allow modern extension features
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("GLEW initialization failed");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//By default there's a bunch of stuff that's disabled in here, can enable them like this
	glEnable(GL_DEPTH_TEST); 

	//create viewport and set up its size
	glViewport(0, 0, bufferWidth, bufferHeight);

	//tell GLFW which class is the "user/owner" of our main window. Since out handlekeys function is static it can't know in the callback
	glfwSetWindowUserPointer(mainWindow, this); 

}

void Window::createCallbacks()
{
	//When key is pressed on the mainWindow, then callBack to the handleKeys function
	glfwSetKeyCallback(mainWindow, handleKeys);

	//when cursor movement detected on mainWindow, it will notify the handleMouse function which we defined
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYchange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}


void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) //if pressed ESC
	{
		//here we can manually set to TRUE that the window should close 
		glfwSetWindowShouldClose(window, GL_TRUE); 
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	//get Access to our window just like the previous (handleKeys) function 
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	//handle the very first mouse movement detected on screen
	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	//update the change in X and Y coordinates
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos; //opposite minus to the last operation in order to avoid inverted camera controls

	//update the last coordinates for next iteration of this function
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}

Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}