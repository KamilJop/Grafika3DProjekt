#pragma once
// Standard Library Includes
#include "stdio.h"

// GL includes
#include <glad\glad.h>
#include <GLFW\glfw3.h>


class Window
{
public:
	Window();

	Window(GLint windowWidth, GLint windowHeight);

	int Initialise();

	GLint getBufferWidth() { return bufferWidth; }
	GLint getBufferHeight() { return bufferHeight; }

	bool getShouldClose() { return glfwWindowShouldClose(mainWindow); }

	bool* getKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	double getScrollY();
	GLFWwindow* getWindow() { return mainWindow; }
	void swapBuffers() { glfwSwapBuffers(mainWindow); }


	~Window();

private:
	GLFWwindow* mainWindow;
	GLint width, height;
	GLint bufferWidth, bufferHeight;

	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	double scrollX;
	double scrollY;

	bool firstMouseMove;

	bool keys[1024];

	static void handleKeys(GLFWwindow* window, int key, int code, int action, int mode);
	static void handleMouse(GLFWwindow* window, double xPos, double yPos);
	static void handleScroll(GLFWwindow* window, double xOffset, double yOffset);
	void createCallbacks();


};

