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
#include "Entity.h"
#include "Light.h"
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

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

// Entity
Entity* triangleEntity;
Entity* floorEntity;

// Light source
Light* mainLight;

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
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11
	};

	GLfloat vertices[] = {
		// positions           // normals
		// Strona 1 (0,3,1)
		-1.0f, -1.0f, 0.0f,     -0.5f, 0.5f, 0.5f,
		 0.0f,  1.0f, 0.0f,     -0.5f, 0.5f, 0.5f,
		 0.0f, -1.0f, 1.0f,     -0.5f, 0.5f, 0.5f,

		 // Strona 2 (1,3,2)
		  0.0f, -1.0f, 1.0f,      0.5f, 0.5f, 0.5f,
		  0.0f,  1.0f, 0.0f,      0.5f, 0.5f, 0.5f,
		  1.0f, -1.0f, 0.0f,      0.5f, 0.5f, 0.5f,

		  // Strona 3 (2,3,0)
		   1.0f, -1.0f, 0.0f,      0.0f, 0.5f, -0.7f,
		   0.0f,  1.0f, 0.0f,      0.0f, 0.5f, -0.7f,
		  -1.0f, -1.0f, 0.0f,      0.0f, 0.5f, -0.7f,

		  // Podstawa (0,1,2)
		  -1.0f, -1.0f, 0.0f,      0.0f, -1.0f, 0.0f,
		   0.0f, -1.0f, 1.0f,      0.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,      0.0f, -1.0f, 0.0f,
	};

	Mesh* mesh1 = new Mesh();
	mesh1->CreateMesh(vertices, indices, 12, 12, 6);
	meshList.push_back(mesh1);

	// Create Entity loading the first mesh and shader
	triangleEntity = new Entity(meshList[0], shaderList[0], glm::vec3(0.0f, 0.0f, -6.5f), glm::vec3(0.0f), glm::vec3(1.0f));

	// Floor
	GLfloat floorVertices[] = {
    // positions             // normals
    -50.0f, -1.0f,  50.0f,    0.0f, 1.0f, 0.0f,
     50.0f, -1.0f,  50.0f,    0.0f, 1.0f, 0.0f,
     50.0f, -1.0f, -50.0f,    0.0f, 1.0f, 0.0f,
    -50.0f, -1.0f, -50.0f,    0.0f, 1.0f, 0.0f
};

	unsigned int floorIndices[] = {
		0, 1, 2,
		2, 3, 0
	};
	Mesh* floorMesh = new Mesh();
	floorMesh->CreateMesh(floorVertices, floorIndices, 12, 6, 6);
	meshList.push_back(floorMesh);

	floorEntity = new Entity(meshList[1], shaderList[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));



	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);


	// Light
	mainLight = new Light(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 5.0f, 0.0f), 0.9f);
	

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

		// Set uniform values
		shaderList[0]->setMat4("projection", projection);
		shaderList[0]->setMat4("view", camera.getViewMatrix());

		// Draw entities
		triangleEntity->DrawEntity();
		floorEntity->DrawEntity();

		// Set light uniforms
		mainLight->useLight(shaderList[0]);

		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}