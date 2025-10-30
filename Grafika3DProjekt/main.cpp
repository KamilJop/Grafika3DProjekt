#include <stdio.h>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include "Window.h"
// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
Window mainWindow;

int main()
{
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();


	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		glfwPollEvents();

		// Clear window
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Testing glm
		glm::vec3 vec(1.0f, 0.0f, 0.0f);

		mainWindow.swapBuffers();
	}

	return 0;
}