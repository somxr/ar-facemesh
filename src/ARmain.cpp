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
	Shader tri_shader("Shaders/triangle_shader.vert", "Shaders/triangle_shader.frag");

	//TRIANGLE//--------------------//

	GLfloat vertices_tri[68*3];
	
	Facemesh facemesh(vertices_tri);

	//GLfloat texture_coord[] =
	//{
	//	0.102f, 0.232f, //0
	//	0.104f, 0.338f,
	//	0.117f, 0.443f,
	//	0.141f, 0.551f,
	//	0.180f, 0.646f,
	//	0.246f, 0.732f, //5
	//	0.322f, 0.809f,
	//	0.410f, 0.865f,
	//	0.519f, 0.881f,
	//	0.609f, 0.864f,
	//	0.695f, 0.803f, //10
	//	0.773f, 0.729f,
	//	0.836f, 0.639f,
	//	0.877f, 0.541f,
	//	0.896f, 0.434f,
	//	0.904f, 0.324f, //15
	//	0.906f, 0.217f,
	//	0.180f, 0.150f,
	//	0.227f, 0.109f,
	//	0.293f, 0.100f,
	//	0.359f, 0.107f, //20
	//	0.426f, 0.135f,
	//	0.559f, 0.131f,
	//	0.625f, 0.100f,
	//	0.695f, 0.090f,
	//	0.765f, 0.100f, //25
	//	0.816f, 0.139f,
	//	0.494f, 0.213f,
	//	0.495f, 0.283f,
	//	0.500f, 0.354f,
	//	0.500f, 0.424f, //30 
	//	0.421f, 0.471f,
	//	0.459f, 0.484f,
	//	0.500f, 0.500f,
	//	0.540f, 0.484f,
	//	0.578f, 0.469f, //35
	//	0.258f, 0.223f,
	//	0.297f, 0.199f,
	//	0.350f, 0.199f,
	//	0.391f, 0.230f,
	//	0.348f, 0.242f, //40
	//	0.297f, 0.240f,
	//	0.600f, 0.227f,
	//	0.643f, 0.193f,
	//	0.693f, 0.193f,
	//	0.734f, 0.216f, //45
	//	0.687f, 0.234f,
	//	0.648f, 0.234f,
	//	0.348f, 0.600f,
	//	0.404f, 0.576f,
	//	0.461f, 0.5625f, //50
	//	0.502f, 0.574f,
	//	0.543f, 0.561f,
	//	0.602f, 0.570f,
	//	0.660f, 0.592f,
	//	0.646f, 0.652f, //55
	//	0.551f, 0.678f,
	//	0.504f, 0.686f,
	//	0.461f, 0.682f,
	//	0.404f, 0.656f,
	//	0.373f, 0.604f, //60
	//	0.461f, 0.598f,
	//	0.500f, 0.602f,
	//	0.543f, 0.596f,
	//	0.635f, 0.598f,
	//	0.547f, 0.627f, //65
	//	0.502f, 0.631f,
	//	0.461f, 0.627f
	//};


	//unsigned int indices[] =
	//{
	//	20, 21, 23,
	//	21, 22, 23,
	//	0, 1, 36,
	//	15, 16, 45,
	//	0, 36, 17,
	//	16, 26, 45,
	//	17, 18, 37, //cw
	//	25, 26, 44, //cw
	//	17, 36, 37,
	//	26, 44, 45,
	//	18, 19, 38, //cw
	//	24, 25, 43, //cw
	//	18, 37, 38,
	//	25, 43, 44,
	//	19, 20, 38, //cw
	//	23, 24, 43, //cw
	//	20, 21, 39, //cw
	//	22, 23, 42, //cw
	//	20, 38, 39,
	//	23, 42, 43,
	//	21, 22, 27, //cw
	//	21, 27, 39, //cw
	//	22, 27, 42,
	//	27, 28, 42,
	//	27, 28, 39, //cw
	//	28, 42, 47, //cw
	//	28, 39, 40,
	//	1,  36, 41, // cw
	//	15, 45, 46,
	//	1,  2, 41, 
	//	14, 15, 46,
	//	28, 29, 40, //cw
	//	28, 29, 47,
	//	2,  40, 41,
	//	14, 46, 47,
	//	2,  29, 40,
	//	14, 29, 47, //cw
	//	2,  3, 29, 
	//	13, 14, 29,
	//	29, 30, 31, //cw
	//	29, 30, 35,
	//	3,  29, 31, //cw
	//	13, 29, 35,
	//	30, 32, 33,
	//	30, 33, 34,
	//	30, 31, 32,
	//	30, 34, 35,
	//	3,  4, 31, 
	//	12, 13, 35,
	//	4,  5, 48, 
	//	11, 12, 54,
	//	5, 6, 48, 
	//	10, 11, 54,
	//	6, 48, 59, //cw
	//	10, 54, 55,
	//	6, 7, 59, 
	//	9, 10, 55,
	//	7, 58, 59,
	//	9, 55, 56,
	//	8, 57, 58,
	//	8, 56, 57,
	//	7, 8, 58, 
	//	8, 9, 56, 
	//	4, 31, 48, //cw
	//	12, 35, 54,
	//	31, 48, 49,
	//	35, 53, 54,
	//	31, 49, 50,
	//	35, 52, 53,
	//	31, 32, 50, //cw
	//	34, 35, 52, //cw
	//	32, 33, 50, //cw
	//	33, 34, 52, //cw
	//	33, 50, 51, 
	//	33, 51, 52, 
	//	49, 48, 60, 
	//	64, 54, 53, 
	//	55, 54, 64,
	//	56, 55, 65,
	//	53, 63, 64,
	//	63, 53, 52,
	//	55, 64, 65,
	//	58, 57, 66,
	//	59, 58, 67,
	//	48, 59, 60,
	//	60, 59, 67,
	//	67, 58, 66,
	//	57, 56, 66,
	//	56, 65, 66,
	//	60, 61, 49, //
	//	49, 61, 50, //
	//	50, 61, 51, //
	//	51, 61, 62, //
	//	62, 63, 51,
	//	51, 63, 52,
	//};

	////GLuint EBO;

	////GLuint VBO_tri_Pos, VBO_tri_texcoord, VAO_tri;
	////glGenVertexArrays(1, &VAO_tri);
	////glGenBuffers(1, &VBO_tri_Pos);

	////glBindVertexArray(VAO_tri);
	////
	////glBindBuffer(GL_ARRAY_BUFFER, VBO_tri_Pos);
	////glBufferData(GL_ARRAY_BUFFER, sizeof(68*3*(sizeof(GLfloat))), NULL, GL_DYNAMIC_DRAW);
	////
	////// Postion Attribute
	////glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	////glEnableVertexAttribArray(0);
	////glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo

	////// Texture Attribute
	////glGenBuffers(1, &VBO_tri_texcoord);
	////glBindBuffer(GL_ARRAY_BUFFER, VBO_tri_texcoord);
	////glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coord), texture_coord, GL_STATIC_DRAW);
	////
	////glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL); 
	////glEnableVertexAttribArray(1);
	////glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo

	//////Index buffer
	////glGenBuffers(1, &EBO);
	////glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	////glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	////glBindVertexArray(0); // Unbind VAO

	facemesh.CreateMesh();

	faceTexture = loadTexture("Textures/UVTEST.png");

	glm::mat4 model_tri;
	//model_tri = glm::translate(model_tri, glm::vec3(0.0f, 0.0f, -2.0f));	//Translate always comes first
	//model_tri *= glm::scale(model_tri, glm::vec3(1.0f, 1.0f, 1.0f)); 


	glm::mat4 view_tri = glm::mat4(1.0f);
	//view_tri = glm::translate(view_tri, glm::vec3(0, 0, -100));

	float nearTriangles = 0.1f;
	float farTriangles = 1000.0f;

	
	//glm::mat4 orthographic_projection_tri = glm::ortho();	
	
	/*glm:: mat4 orthographic_projection_tri =
		glm::ortho(-100.0f,
			100.0f,
			(-100.0f * ((GLfloat)height_window / (GLfloat)width_window)),
			(100.0f * ((GLfloat)height_window / (GLfloat)width_window)),
			-100.0f,
			100.0f);
	*/

	glm::mat4 orthographic_projection_tri = glm::ortho(0.0f, (GLfloat)width_window, 0.0f, (GLfloat)height_window, -100.0f, 100.0f);

	///BACKGROUND DEFINITIONS///
	cv::Mat frame, framePrev;

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

	float nearPlane = 0.1f;
	//float far = 500.0f;

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
	glm::mat4 orthographic_projection_bg = glm::ortho(0.0f, (GLfloat)width_window, 0.0f, (GLfloat)height_window, nearPlane, ortho_far);
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
		dlib::cv_image<dlib::bgr_pixel> dlibImage(frame);


		if (frameCounter % skipFrames == 0)
		{
			faces = faceDetector(dlibImageSmall);

			//if a face is detected, process landmarks
			//working with just the first detected face for simplicity
			if(faces.size() > 0)
			{
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
				int index = 0;

				for (int j = 0; j < 68; j++)
				{
					int x = faceLandmark.part(j).x();
					int y = faceLandmark.part(j).y();

					//vertices_tri[index] = x * (width_window/float(width)); //get the pixel coordinate of the landmark and scale it to fit the window width and projection matrix
					//vertices_tri[index + 1] = (height - y) * (height_window/float(height)); //same as the x coordinate but first minus the y from height because opencv y axis is flipped
					//vertices_tri[index + 2] = 1.0f;

					//index += 3;

					points.push_back(cv::Point2f(x, y));

					cv::Point point = cv::Point(x, y);
					//cv::circle(frame, point, 3, cv::Scalar(0, 0, 255));
				}

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


				for (int j = 0; j < 68; j++)
				{
					float smoothX = 1.0f * pointsPrev[j].x + 0.0f * pointsPredicted[j].x;
					float smoothY = 1.0f * pointsPrev[j].y + 0.0f * pointsPredicted[j].y;

					cv::Point point = cv::Point(smoothX, smoothY);
					cv::circle(frame, point, 3, cv::Scalar(0, 0, 255));

					/*vertices_tri[index] = smoothX * (width_window / float(width));
					vertices_tri[index + 1] = (height - smoothY) * (height_window / float(height));
					vertices_tri[index + 2] = 1.0f;*/
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

		frameCounter++;
		if (frameCounter == 100)
		{
			frameCounter = 0;
		}

		//wait
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
			model_tri = glm::mat4(1.0f);
			view_tri = glm::mat4(1.0f);

			model_tri = glm::translate(model_tri, glm::vec3(0, 0, -100.0f));
			

			//DRAW Facemesh
			glUseProgram(tri_shader.program);
			glUniformMatrix4fv(glGetUniformLocation(tri_shader.program, "model_tri"), 1, GL_FALSE, glm::value_ptr(model_tri));
			glUniformMatrix4fv(glGetUniformLocation(tri_shader.program, "view_tri"), 1, GL_FALSE, glm::value_ptr(view_tri));
			glUniformMatrix4fv(glGetUniformLocation(tri_shader.program, "orthographic_projection_tri"), 1, GL_FALSE, glm::value_ptr(orthographic_projection_bg));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, faceTexture);

			//glBindVertexArray(VAO_tri);

			//glPointSize(5.0f);
			//glBindBuffer(GL_ARRAY_BUFFER, VBO_tri_Pos);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_tri), vertices_tri, GL_DYNAMIC_DRAW);
			//glBindBuffer(GL_ARRAY_BUFFER, 0);

			////glDrawArrays(GL_TRIANGLE_STRIP, 0, 68);
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			//glDrawElements(GL_TRIANGLES, 285, GL_UNSIGNED_INT, 0);

			////glDrawArrays(GL_POINTS, 0, 68);

			//glBindVertexArray(0);

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

