#ifdef _DEBUG
#if defined(WIN32)||defined(_WIN32)||defined(__WIN32)&&!defined(__CYGWIN__)
#include <windows.h>
#include <wincon.h>		// for debugging console
#endif
#endif

#define GLEW_STATIC
#include <thread>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ShaderUtil.h"

void glfw_error_callback(int error, const char* description)
{
#ifdef _DEBUG
	puts(description);
#endif
}

int main()
{
#ifdef _DEBUG
	//AllocConsole();
	std::cout << "Debugging Window:\n" << std::endl;
#endif

	glfwSetErrorCallback(glfw_error_callback);

	if (!glfwInit())
	{
#ifdef _DEBUG
		std::cout << "glfwInit() failed" << std::endl;
#endif
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);						// OpenGL3.2
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);		// specify only support new core functionality of OpenGL3.2
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", nullptr, nullptr);	// width, height, window title, null for window mode|glfwGetPrimaryMonitor() for full screen, OpenGL context to share resource
	//GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr);
	if (window == NULL)
	{
#ifdef _DEBUG
		std::cout << "glfwCreateWindow() cannot create window. Check hardware support" << std::endl;
#endif
	}

	glfwMakeContextCurrent(window);
	
	glewExperimental = GL_TRUE;
	GLenum err=glewInit();
	if (err != GLEW_OK)
	{
#ifdef _DEBUG
		std::cout << "glewInit() failed, aborting. Code " << err << ". Error: " << glewGetErrorString(err) << std::endl;
#endif
	}

	// Vertex Array Objects, use to link VBO and attributes with raw vertex data
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create Vertex Buffer
	GLuint vbo;
	glGenBuffers(1, &vbo);
	// Define vertices. Move to seperate function later
	float vertices[] = {
		// X      Y     Z     R     G     B     U     V
		// start of the cube
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		// end of the cube

		// start of the floor
		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f
		// end of the floor
	};
	// Make the vbo active object
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	// Draw, referto to binded vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);		// GL_STATIC_DRAW, GL_DYNAMIC_DRAW, or GL_STREAM_DRAW

//	// Define elements
//	GLuint elements[] = {
//		0, 1, 2,
//		2, 3, 0
//	};
//	GLuint ebo;
//	glGenBuffers(1, &ebo);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);


	// Create and Apply Vertex Shader object handler
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	std::string vertexShaderStr = readShaderFile("VertexShader.vert");
	const char *vertexShaderSrc = vertexShaderStr.c_str();
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);
#ifdef _DEBUG	// Check Shader Compilation Error
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	char errorBuf[512];
	glGetShaderInfoLog(vertexShader, sizeof(errorBuf), NULL, errorBuf);
	std::cout << errorBuf << std::endl;
#endif
	// Create and Apply Fragment Shade
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string fragmentShaderStr = readShaderFile("FragmentShader.frag");
	const char *fragmentShaderSrc = fragmentShaderStr.c_str();
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);
#ifdef _DEBUG	// Check Shader Compilation Error
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	glGetShaderInfoLog(fragmentShader, sizeof(errorBuf), NULL, errorBuf);
	std::cout << errorBuf << std::endl;
#endif


	// Combining shaders into program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");	// for fragmentShader apply to multiple buffers
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);
	
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// Varying Triangle Color
	//GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
	//glUniform3f(uniColor, 0.0f, 0.0f, 0.0f);
	//auto t_start = std::chrono::high_resolution_clock::now();


	// Textures
	GLuint textures[2];
	glGenTextures(2, textures);
	int width, height;
	unsigned char* image;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	image = SOIL_load_image("image/kitten.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	image = SOIL_load_image("image/puppy.png", &width, &height, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Setup MVP outside the loop. Model need to calc everytime inside the loop
	GLint uniModel = glGetUniformLocation(shaderProgram, "model");
	GLint uniView = glGetUniformLocation(shaderProgram, "view");
	GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
	// View transformation
	glm::mat4 view = glm::lookAt(
		glm::vec3(2.5f, 2.5f, 2.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	// Projection transform
	glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);	//FOV, aspect ratio of screen, near, far clipping plane
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

	// Blending set timer
	GLint blendRatio = glGetUniformLocation(shaderProgram, "blendRatio");
	glUniform1f(blendRatio, 0.0f);
	auto t_start = std::chrono::high_resolution_clock::now();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		//glEnable(GL_DEPTH_TEST);		// this will fill the depth buffer with zeros, black screen

		// Blending
		auto t_now = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		glUniform1f(blendRatio, sin(time*4.0f) / 2.0f + 0.5f);

		// keep rotate with time
		glm::mat4 model;
		model = glm::rotate(
			model,
			time * glm::radians(180.0f) ,
			glm::vec3(0.0f, 0.0f, 1.0f)
		);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));	// have to update to the handler everytime

		// Clear the screen to black
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, 36);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glEnable(GL_STENCIL_TEST);
			// Draw floor
			glStencilFunc(GL_ALWAYS, 1, 0xFF); // Set any stencil to 1
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF); // Write to stencil buffer
			glDepthMask(GL_FALSE); // Don't write to depth buffer
			glClear(GL_STENCIL_BUFFER_BIT); // Clear stencil buffer (0 by default)
			glDrawArrays(GL_TRIANGLES, 36, 6);

			// Draw cube reflection
			glStencilFunc(GL_EQUAL, 1, 0xFF);
			glStencilMask(0x00); // Write nothing to stencil buffer
			glDepthMask(GL_TRUE); // why? write to depth buffer
			model = glm::translate(model, glm::vec3(0, 0, -1));
			model = glm::scale(model, glm::vec3(1, 1, -1));
			glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

			GLint overrideColor = glGetUniformLocation(shaderProgram, "overrideColor");
			glUniform3f(overrideColor, 0.3f, 0.3f, 0.3f);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glUniform3f(overrideColor, 1.0f, 1.0f, 1.0f);


		glDisable(GL_STENCIL_TEST);

		// Triangle color keep varying
		//auto t_now = std::chrono::high_resolution_clock::now();
		//float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
		//glUniform3f(uniColor, (sin(time*4.0f) + 1.0f / 2.0f), (sin(time*2.0f) + 1.0f / 2.0f), (sin(time*1.0f) + 1.0f / 2.0f));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteTextures(2, textures);

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);

	glDeleteBuffers(1, &vbo);

	glDeleteVertexArrays(1, &vao);


	glfwTerminate();
	return 0;
}