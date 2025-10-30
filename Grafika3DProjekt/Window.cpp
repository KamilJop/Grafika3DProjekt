#include "Window.h"


// Default constructor
Window::Window()
{
	width = 1280;
	height = 720;
}

// Constructor with parameters
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
}


// Initialise the window
int Window::Initialise() {

	// Initialise GLFW
	if (!glfwInit())
	{
		printf("GLFW initialisation failed!");
		glfwTerminate();
		return 1;
	}

	// Set OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Set profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	// Create window
	mainWindow = glfwCreateWindow(width, height, "Gra", NULL, NULL);
	if (!mainWindow)
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return 1;
	}

	// Get buffer size 
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	// Set context
	glfwMakeContextCurrent(mainWindow);


	// Check and initialise GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GLAD initialisation failed!");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Set viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	return 0;

}

// Destructor
Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}