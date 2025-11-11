#include "Window.h"


// Default constructor
Window::Window()
{
	width = 1280;
	height = 720;

	for(size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}

	lastX = width / 2.0f;
	lastY = height / 2.0f;
	firstMouseMove = true;
	xChange = 0.0f;
	yChange = 0.0f;

}

// Constructor with parameters
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = false;
	}


	lastX = width / 2.0f;
	lastY = height / 2.0f;
	firstMouseMove = true;
	xChange = 0.0f;
	yChange = 0.0f;
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

	// Capture the mouse
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the user pointer to window instance
	glfwSetWindowUserPointer(mainWindow, this);

	// Create the callbacks
	createCallbacks();


	return 0;

}


// Handle keyboard input

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode)
{	
	// Get the pointer to the Window class
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	// Close the window if ESC is pressed
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	// Update the keys array if keys pressed/released
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

// Handle mouse movement
void Window::handleMouse(GLFWwindow* window, double xPos, double yPos)
{
	// Get the pointer to the Window class
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	// Check for first mouse input
	if (theWindow->firstMouseMove)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->firstMouseMove = false;
	}
	// Calculate the change in mouse position
	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;
	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


// Setup the callbacks
void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

// Getter for mouse x position change
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

// Getter for mouse y position change
GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

// Destructor
Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}
