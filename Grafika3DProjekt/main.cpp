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
#include "DirectionalLight.h"
#include "Material.h"

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
DirectionalLight* mainLight;

// Materials
Material* shinyMaterial;
Material* lessShinyMaterial;

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
	GLfloat vertices[] = {
		 0.0f,  1.0f,  0.0f,       0.0f,    0.980f,  0.196f,  
		-1.0f, -1.0f,  0.0f,      -0.928f, 0.0f,   -0.371f, 
		 0.0f, -1.0f,  1.0f,       0.0f,    0.0f,    1.0f,    
		 1.0f, -1.0f,  0.0f,       0.928f,  0.0f,   -0.371f   
	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
		0, 3, 1,
		1, 2, 3
	};

	Mesh* mesh1 = new Mesh();
	mesh1->CreateMesh(vertices, indices, 12, 12, 6);
	meshList.push_back(mesh1);

	// Create Materials
	shinyMaterial = new Material(1.0f, 64.0f);
	lessShinyMaterial = new Material(0.5f, 128.0f);

	// Create Entity loading the first mesh and shader
	triangleEntity = new Entity(meshList[0], shaderList[0], glm::vec3(0.0f, 0.0f, -6.5f), glm::vec3(0.0f), glm::vec3(1.0f), shinyMaterial);

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


	floorEntity = new Entity(meshList[1], shaderList[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f), lessShinyMaterial);



	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);


	// Light
	mainLight = new DirectionalLight(glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(2.0f, -1.0f, -4.0f), 0.15f, 0.8f);
	

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
		shaderList[0]->setVec3("cameraPosition", camera.getCameraPosition());

		// Set light uniforms
		mainLight->useLight(shaderList[0]);

		// Draw entities
		triangleEntity->DrawEntity();
		floorEntity->DrawEntity();



		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}