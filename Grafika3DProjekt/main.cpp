#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include <vector>
#include "Camera.h"

// Window dimensions
const GLint WIDTH = 1280, HEIGHT = 720;

// Create window object
Window mainWindow;

// Create mesh and shader lists
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;

// Shader file paths
static const char* vertexShader = "Shaders/shader.vert";
static const char* fragmentShader = "Shaders/shader.frag";

// Uniform locations
GLuint uniformProjection;
GLuint uniformModel;
GLuint uniformViewMatrix;

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);


int main()
{
	// Create Window
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();
	

	// Create Shaders
	Shader* shader1 = new Shader();
	shader1->CreateShader(vertexShader, fragmentShader);
	shaderList.push_back(shader1);

	// Create Meshes
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	Mesh* mesh1 = new Mesh();
	mesh1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(mesh1);

	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Model matrix
	glm::mat4 model;
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, -6.5f));

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Get + Handle user input events
		glfwPollEvents();

		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use shader program
		shaderList[0]->UseShader();

		// Camera movement
		camera.ProcessKeyboard(mainWindow.getKeys(), deltaTime);
		camera.ProcessMouseMovement(mainWindow.getXChange(), mainWindow.getYChange());

		

		// Set uniform locations and values

		uniformModel = shaderList[0]->getModelUniformLocation();
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		uniformProjection = shaderList[0]->getProjectionUniformLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		uniformViewMatrix = shaderList[0]->getViewMatrixUniformLocation();
		glUniformMatrix4fv(uniformViewMatrix, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

		
		// Render mesh
		meshList[0]->RenderMesh();

		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}