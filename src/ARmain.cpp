#pragma once
#define STB_IMAGE_IMPLEMENTATION
//#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/opencv.hpp>

//#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include "SOIL.h"

// some extra required imports
#include <string.h>
#include <vector>
#include <math.h>

//MINE
#include "Shader.h"
#include "Object.h"
#include "Facemesh.h"
//#include "Texture.h"

//DLIB
#include<dlib/image_processing/frontal_face_detector.h>
#include<dlib/image_processing.h>
#include<dlib/opencv.h>

#include "stb_image.h"

//Global vairables

int width_window = 640, height_window = 480;
GLuint faceTexture;

GLuint loadTexture(const char* filename);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	// When the user presses the escape key, we set the window should close property to true, and close the application.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

GLFWwindow* initializeOpenglWindow(int width_window, int height_window)
{
	// =========================================================================================================
	// OpenGL initializations
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(width_window, height_window, "AR Camera", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	if (window == nullptr)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return NULL;
	}
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}

	glEnable(GL_DEPTH_TEST);

	return window;
}

std::vector<cv::Point2f> getFaceLandmarks(std::vector<dlib::rectangle>& faces, float resizeScale, cv::Mat& frame, dlib::shape_predictor& landmarkDetector)
{
	dlib::cv_image<dlib::bgr_pixel> dlibImage(frame);
	
	//scale the rectangle coordinates as we did face detection on resized smaller image
	dlib::rectangle rect(int(faces[0].left() * resizeScale),
		int(faces[0].top() * resizeScale),
		int(faces[0].right() * resizeScale),
		int(faces[0].bottom() * resizeScale));

	cv::Point topLeft = cv::Point(faces[0].left(), faces[0].top());
	cv::Point bottomRight = cv::Point(faces[0].right(), faces[0].bottom());

	cv::rectangle(frame, topLeft, bottomRight, cv::Scalar(255, 0, 0));

	//Face landmark detection
	dlib::full_object_detection faceLandmark = landmarkDetector(dlibImage, rect);

	std::vector<cv::Point2f> points;

	for (int j = 0; j < 68; j++)
	{
		int x = faceLandmark.part(j).x();
		int y = faceLandmark.part(j).y();

		points.push_back(cv::Point2f(x, y));

		cv::Point point = cv::Point(x, y);
		//cv::circle(frame, point, 3, cv::Scalar(0, 0, 255));
	}

	return points;
}

int main(int argc, char** argv)
{
	GLFWwindow* window = initializeOpenglWindow(width_window, height_window);

	if (window == NULL)
	{
		return -1;
	}

	// Viewport dimensions
	glfwGetFramebufferSize(window, &width_window, &height_window);
	glViewport(0, 0, width_window, height_window);

	Shader bg_shader("Shaders/bg_vertex_shader.vert", "Shaders/bg_fragment_shader.frag");
	Shader face_shader("Shaders/triangle_shader.vert", "Shaders/triangle_shader.frag");

	//Facemesh//--------------------//
	
	Facemesh facemesh;
	facemesh.CreateMesh();

	faceTexture = loadTexture("Textures/UVTEST.png");

	glm::mat4 model_face;
	glm::mat4 view_face = glm::mat4(1.0f);

	float nearPlane_face = 0.1f;
	float farPlane_face = 1000.0f;
	glm::mat4 orthographic_projection_face = glm::ortho(0.0f, (GLfloat)width_window, 0.0f, (GLfloat)height_window, -100.0f, 100.0f);

	///BACKGROUND DEFINITIONS///
	cv::Mat frame;

	cv::Mat frameGray, frameGrayPrev;

	GLfloat vertices_bg[] =
	{
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	};

	GLuint VBO_bg, VAO_bg;
	glGenVertexArrays(1, &VAO_bg);
	glGenBuffers(1, &VBO_bg);

	glBindVertexArray(VAO_bg);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_bg), vertices_bg, GL_STATIC_DRAW);

	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Texture Attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO_bg

	// -----------------------------------------------------------------------------------------------
	// webcam texture

	float nearPlane_bg = 0.1f;

	int width, height;
	cv::VideoCapture cap(1);

	if (!cap.isOpened()) {
		std::cerr << "ERROR: Could not open camera" << std::endl;
		return 1;
	}

	bool firstFrame = true;
	cap >> frame;
	width = frame.size().width;
	height = frame.size().height;

	std::cout << width << std::endl; //TESTING

	GLuint texture_bg;
	glGenTextures(1, &texture_bg);
	glBindTexture(GL_TEXTURE_2D, texture_bg);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, frame.data);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// =========================================================================================================
	// transformation presets for the background object (model, view and orthographic projection)
	glm::mat4 model_bg(1.0f);
	glm::mat4 view_bg(1.0f);
	GLfloat ortho_far = 10000.0f;

	model_bg = glm::scale(model_bg, glm::vec3(width_window, height_window, 1));
	view_bg = glm::translate(view_bg, glm::vec3(0, 0, -ortho_far));

	// use orthographic projection for the webcam background
	glm::mat4 orthographic_projection_bg = glm::ortho(0.0f, (GLfloat)width_window, 0.0f, (GLfloat)height_window, nearPlane_bg, ortho_far);
	// =========================================================================================================

	//////////DLIB LANDMARKS STUFF////////////// 
	//define the face detector
	dlib::frontal_face_detector faceDetector = dlib::get_frontal_face_detector();

	//define landmark detector
	dlib::shape_predictor landmarkDetector;

	//load face landmark model
	dlib::deserialize("shape_predictor_68_face_landmarks.dat") >> landmarkDetector;

	//define resize height
	float resizeHeight = 480;
	
	//calculate resize scale
	float resizeScale = frame.rows / resizeHeight;

	//define to hold detected faces
	std::vector<dlib::rectangle> faces;
	int frameCounter = 0;
	int skipFrames = 2;


	std::vector<cv::Point2f> pointsPredicted;
	std::vector<cv::Point2f> pointsPrev;
	 
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cap >> frame;

		//smaller image frame for more effecient face recognition 
		cv::Mat smallFrame; 
		cv::resize(frame, smallFrame, cv::Size(), 1.0 / resizeScale, 1.0 / resizeScale); 

		//change to dlib image format
		dlib::cv_image<dlib::bgr_pixel> dlibImageSmall(smallFrame);
		
		//skip frames when detecting face for effeciency, faces dont move fast enough to need detection every frame
		if (frameCounter % skipFrames == 0)
		{
			faces = faceDetector(dlibImageSmall);

			//if a face is detected, process landmarks
			//working with just the first detected face
			if(faces.size() > 0)
			{
				std::vector<cv::Point2f> points = getFaceLandmarks(faces, resizeScale, frame, landmarkDetector);

				cv::cvtColor(frame, frameGray, cv::COLOR_BGR2GRAY);

				if (firstFrame)
				{
					frameGrayPrev = frameGray.clone();
					pointsPrev = points;
					firstFrame = false;
				}

				cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT)+(cv::TermCriteria::EPS), 20, 0.001);
				std::vector<uchar> status;
				std::vector<float> err;
				cv::calcOpticalFlowPyrLK(frameGrayPrev, frameGray, pointsPrev, pointsPredicted, status, err, cv::Size(101, 101), 5, criteria);

				int index = 0;

				for (int j = 0; j < 68; j++)
				{
					float smoothX = 0.3f * pointsPrev[j].x + 0.7f * pointsPredicted[j].x;
					float smoothY = 0.3f * pointsPrev[j].y + 0.7f * pointsPredicted[j].y;

					GLfloat x = smoothX * (width_window / float(width));
					GLfloat y = (height - smoothY) * (height_window / float(height));
					GLfloat z = 1.0f;

					facemesh.updateVertices(index, x, y, z);

					index += 3;
				}

				std::swap(frameGrayPrev, frameGray);
				std::swap(pointsPrev, points);
			
			}
		}

		//update frame counter and reset at 100
		frameCounter++;
		if (frameCounter == 100)
		{
			frameCounter = 0;
		}

		//Draw background
		glUseProgram(bg_shader.program);
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "model_bg"), 1, GL_FALSE, glm::value_ptr(model_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "view_bg"), 1, GL_FALSE, glm::value_ptr(view_bg));
		glUniformMatrix4fv(glGetUniformLocation(bg_shader.program, "orthographic_projection_bg"), 1, GL_FALSE, glm::value_ptr(orthographic_projection_bg));

		glBindVertexArray(VAO_bg);

		glActiveTexture(GL_TEXTURE2);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.data);
		glBindTexture(GL_TEXTURE_2D, texture_bg);
		glUniform1i(glGetUniformLocation(bg_shader.program, "webcam_texture"), 2);

		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		glUseProgram(0);
		// -----------------------------------------------------------------------------------------------------

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//if a face is detected, render the face mesh using the VAO
		if (faces.size() > 0) {
			//Matrix should be here
			model_face = glm::mat4(1.0f);
			view_face = glm::mat4(1.0f);

			model_face = glm::translate(model_face, glm::vec3(0, 0, -100.0f));
			

			//DRAW Facemesh
			glUseProgram(face_shader.program);
			glUniformMatrix4fv(glGetUniformLocation(face_shader.program, "model_tri"), 1, GL_FALSE, glm::value_ptr(model_face));
			glUniformMatrix4fv(glGetUniformLocation(face_shader.program, "view_tri"), 1, GL_FALSE, glm::value_ptr(view_face));
			glUniformMatrix4fv(glGetUniformLocation(face_shader.program, "orthographic_projection_tri"), 1, GL_FALSE, glm::value_ptr(orthographic_projection_bg));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, faceTexture);

			facemesh.Render();
		}

		glUseProgram(0); 
		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);

	glfwTerminate();
	return EXIT_SUCCESS;
}


//Texture 
GLuint loadTexture(const char* filename)
{
	// Step1 Create and bind textures
	GLuint textureId = 0;
	glGenTextures(1, &textureId);
	assert(textureId != 0);

	glBindTexture(GL_TEXTURE_2D, textureId);

	// Step2 Set filter parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Step3 Load Textures with dimension data
	int width, height, nrChannels;
	unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
	if (!data)
	{
		std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
		return 0;
	}

	// Step4 Upload the texture to the PU
	GLenum format = 0;
	if (nrChannels == 1)
		format = GL_RED;
	else if (nrChannels == 3)
		format = GL_RGB;
	else if (nrChannels == 4)
		format = GL_RGBA;
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
		0, format, GL_UNSIGNED_BYTE, data);

	// Step5 Free resources
	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureId;
}

