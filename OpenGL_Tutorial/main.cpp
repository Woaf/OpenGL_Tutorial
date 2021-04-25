#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

static void SetGLFWMetadata ()
{
	glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
}

static void HandleKeyInput (GLFWwindow* window, int key, int status, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose (window, GLFW_TRUE);
	} else {
		std::cout << "Key code: " << key << "\taction: " << action << "\tmods: " << mods << std::endl;
	}
}

constexpr float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f, 0.5f, 0.0f
};

std::string ReadFile (const std::string& fileLocation)
{
	std::ifstream file;
	file.open (fileLocation, std::ios::in);
	std::stringstream contents;
	if (!file.fail ()) {
		contents << file.rdbuf ();
	}

	return contents.str ();
}

GLuint CreateAndCompileShader (const std::string& fileLoc, GLuint shaderType)
{
	const std::string shaderSource = ReadFile (fileLoc);
	const char* cShader = shaderSource.c_str ();

	GLuint shader = glCreateShader (shaderType);
	glShaderSource (shader, 1, &cShader, nullptr);
	glCompileShader (shader);

	int success;
	glGetShaderiv (shader, GL_COMPILE_STATUS, &success);
	if (success == 0) {
		char infoLog[512];
		glGetShaderInfoLog (shader, 512, nullptr, infoLog);
		std::cerr << "Shader error: " << infoLog << std::endl;
	}

	return shader;
}

GLuint CreateAndLinkProgram ()
{
	GLuint vertexShader = CreateAndCompileShader ("../../../../shaders/shader.vert", GL_VERTEX_SHADER);
	GLuint fragmentShader = CreateAndCompileShader ("../../../../shaders/shader.frag", GL_FRAGMENT_SHADER);

	GLuint program = glCreateProgram ();
	glAttachShader (program, vertexShader);
	glAttachShader (program, fragmentShader);

	glLinkProgram (program);

	int success;
	glGetProgramiv (program, GL_LINK_STATUS, &success);
	if (success == 0) {
		char infoLog[512];
		glGetProgramInfoLog (program, 512, nullptr, infoLog);
		std::cerr << "Program error: " << infoLog << std::endl;
	}

	return program;
}

int main()
{
	if (!glfwInit ()) {
		std::cerr << "GLFW init failed..." << std::endl;
		return -1;
	}

	SetGLFWMetadata ();

	GLFWwindow* window = glfwCreateWindow (720, 480, "OpenGL tutorial", nullptr, nullptr);

	if (window == nullptr) {
		std::cerr << "Window init failed..." << std::endl;
		glfwTerminate ();
		return -2;
	}

	glfwMakeContextCurrent (window);
	glfwSetKeyCallback (window, HandleKeyInput);

	if (glewInit () != GLEW_OK) {
		std::cerr << "Glew init failed..." << std::endl;
		glfwDestroyWindow (window);
		glfwTerminate ();
		return -3;
	}

	GLuint shaderProgram = CreateAndLinkProgram ();
	GLuint VBO, VAO;
	glGenVertexArrays (1, &VAO);
	glGenBuffers (1, &VBO);
	glBindVertexArray (VAO);
	glBindBuffer (GL_ARRAY_BUFFER, VBO);
	glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 3* sizeof (float), (void*)0);
	glEnableVertexAttribArray (0);

	glBindBuffer (GL_ARRAY_BUFFER, 0);
	glBindVertexArray (0);

	glClearColor (0.2f, 0.2f, 0.45f, 1.0f);

	int width;
	int height;
	while (!glfwWindowShouldClose (window)) {
		glfwPollEvents ();

		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwGetFramebufferSize (window, &width, &height);
		glViewport (0, 0, width, height);

		glUseProgram (shaderProgram);
		glBindVertexArray (VAO);
		glDrawArrays (GL_TRIANGLES, 0, 3);

		glfwSwapBuffers (window);
	}

	glDeleteVertexArrays (1, &VAO);
	glDeleteBuffers (1, &VBO);
	glDeleteProgram (shaderProgram);

	/*optional*/
	glfwDestroyWindow (window);
	glfwTerminate ();
	/*--------*/

	return 0;
}
