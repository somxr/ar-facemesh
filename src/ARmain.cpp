#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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

//DLIB
#include<dlib/image_processing/frontal_face_detector.h>
#include<dlib/image_processing.h>
#include<dlib/opencv.h>

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


int width_window = 640, height_window = 480;

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

	/*float vertices_tri[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};*/

	GLfloat texture_coord[] = 
	{
		0.0f, 0.0f,
		0.5f, 0.0f,
		1.0f, 0.0f
	}; 

	GLuint VBO_tri_Pos, VBO_tri_texcoord, VAO_tri;
	glGenVertexArrays(1, &VAO_tri);
	glGenBuffers(1, &VBO_tri_Pos);

	glBindVertexArray(VAO_tri);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tri_Pos);
	glBufferData(GL_ARRAY_BUFFER, sizeof(68*3*(sizeof(float))), NULL, GL_DYNAMIC_DRAW);
	
	// Postion Attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo

	// Texture Attribute
	glGenBuffers(1, &VBO_tri_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_tri_texcoord);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texture_coord), texture_coord, GL_STATIC_DRAW);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL); 
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind vbo
	glBindVertexArray(0); // Unbind VAO

	glm::mat4 model_tri;
	model_tri = glm::translate(model_tri, glm::vec3(0.0f, 0.0f, -2.0f));	//Translate always comes first
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
	cv::Mat frame;

	GLfloat vertices_bg[] =
	{
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		1.0f, 1.0f, 0.0f,  1.0f, 1.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,  0.0f, 0.0f,
	};

	//Object sphere = Object("Models/sphere.obj");

	//unsigned int indices_pyramid[] = {
	//	0, 3, 1,
	//	1, 3, 2,
	//	2, 3, 0,
	//	0, 1, 2
	//};
	
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

	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();
		//glClearColor(0.27f, 0.27f, 0.27f, 1.0f);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cap >> frame;

		//smaller image frame for more effecient face recognition 
		cv::Mat smallFrame; 
		cv::resize(frame, smallFrame, cv::Size(), 1.0 / resizeScale, 1.0 / resizeScale); 

		//change to dlib image format
		dlib::cv_image<dlib::bgr_pixel> dlibImageSmall(smallFrame);
		dlib::cv_image<dlib::bgr_pixel> dlibImage(frame);

		faces = faceDetector(dlibImageSmall);

		//loop over faces
		for (int i = 0; i < faces.size(); i++) {




			//scale the rectangle coordinates as we did face detection on resized smaller image
			dlib::rectangle rect(int(faces[i].left() * resizeScale),
				int(faces[i].top() * resizeScale),
				int(faces[i].right() * resizeScale),
				int(faces[i].bottom() * resizeScale));

			cv::Point topLeft = cv::Point(faces[i].left(), faces[i].top());
			cv::Point bottomRight = cv::Point(faces[i].right(), faces[i].bottom());

			cv::rectangle(frame, topLeft,bottomRight, cv::Scalar(255, 0, 0));

			//Face landmark detection
			dlib::full_object_detection faceLandmark = landmarkDetector(dlibImage, rect);

			int index = 0;
			for (i = 0; i < 68; i++)
			{
				
				int x = faceLandmark.part(i).x();
				int y = faceLandmark.part(i).y();

				vertices_tri[index] = x;
				vertices_tri[index + 1] = width - y;



				//std::cout << y << std::endl;

				vertices_tri[index + 2] = 1.0f;
				index += 3;
				//std::cout << faceLandmark.part(i).x()/float(width) << std::endl; //TESTING
				cv::Point point = cv::Point(x, y);
				cv::circle(frame, point, 3, cv::Scalar(0, 0, 255));
			}


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

		

		//Matrix should be here
		model_tri = glm::mat4(1.0f);
		view_tri = glm::mat4(1.0f);

		model_tri = glm::translate(model_tri, glm::vec3(0, 0, -50.0f));
		//model_tri = glm::scale(model_tri, glm::vec3(3.0f, 3.0f, 3.0f));
		//DRAW TRIANGLE

		
			glUseProgram(tri_shader.program);
			glUniformMatrix4fv(glGetUniformLocation(tri_shader.program, "model_tri"), 1, GL_FALSE, glm::value_ptr(model_tri));
			glUniformMatrix4fv(glGetUniformLocation(tri_shader.program, "view_tri"), 1, GL_FALSE, glm::value_ptr(view_tri));
			glUniformMatrix4fv(glGetUniformLocation(tri_shader.program, "orthographic_projection_tri"), 1, GL_FALSE, glm::value_ptr(orthographic_projection_bg));

			glBindVertexArray(VAO_tri);	


			glPointSize(10.0f);
			glBindBuffer(GL_ARRAY_BUFFER, VBO_tri_Pos);
			glBufferData(GL_ARRAY_BUFFER, sizeof(68 * 3 * (sizeof(float))), vertices_tri, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_POINTS, 0, 68);
			glBindVertexArray(0);

		
		glUseProgram(0); 

		glfwSwapBuffers(window);

	}

	glDeleteVertexArrays(1, &VAO_bg);
	glDeleteBuffers(1, &VBO_bg);

	glfwTerminate();
	return EXIT_SUCCESS;

}