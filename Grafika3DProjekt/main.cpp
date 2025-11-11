#define STB_IMAGE_IMPLEMENTATION

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
#include "PointLight.h"
#include "Flashlight.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include "Model.h"


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
static const char* lightVertexShader = "Shaders/light_source.vert";   
static const char* lightFragmentShader = "Shaders/light_source.frag";

// Texture file paths
static const char* brickTexture = "Textures/brick.png";
static const char* stoneTexture = "Textures/stone.png";

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

// Entity
Entity* doorEntity;
Entity* floorEntity;
Entity* xwingEntity;

// Light source
DirectionalLight* mainLight;
PointLight* pointLight;
PointLight* pointLight2;
PointLight* pointLight3;
Flashlight* flashlight;

// Materials
Material* shinyMaterial;
Material* lessShinyMaterial;


// Create models
Model door;
Model floorModel;
Model xwing;

int main()
{
	// Create Window
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	
	// Create Shaders
	Shader* shader1 = new Shader();
	shader1->CreateShader(vertexShader, fragmentShader);
	shaderList.push_back(shader1);

	Shader* lightShader = new Shader();
	lightShader->CreateShader(lightVertexShader, lightFragmentShader); 
	shaderList.push_back(lightShader);


	// Load Models
	door = Model();
	door.LoadModel("Models/door.obj");
	floorModel = Model();
	floorModel.LoadModel("Models/Cranberry_Doormat.obj");


	// Create Materials
	shinyMaterial = new Material(1.0f, 64.0f);
	lessShinyMaterial = new Material(0.5f, 128.0f);

	// Create Entities
	doorEntity = new Entity(&door, shinyMaterial, glm::vec3(0.0f, -1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.8f));
	floorEntity = new Entity(&floorModel, lessShinyMaterial, glm::vec3(0.0f, -1.5f, -3.0f), glm::vec3(0.0f), glm::vec3(0.5f));


	glm::vec3 rotation = glm::vec3(0.0f);

	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);


	// Light
	mainLight = new DirectionalLight(glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(2.0f, -1.0f, -4.0f), 0.15f, 0.8f);
	pointLight = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 0.9f, glm::vec3(0.0f, 1.0f, -3.5f), 1.0f, 0.09f, 0.032f,0);
	pointLight2 = new PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.5f, 0.9f, glm::vec3(-3.5f, 0.5f, -4.0f), 1.0f, 0.12f, 0.062f, 1);
	pointLight3 = new PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, 0.9f, glm::vec3(3.5f, 0.5f, -4.0f), 1.0f, 0.12f, 0.062f, 2);
	flashlight = new Flashlight(glm::vec3(1.0f, 1.0f, 0.85f), 0.02f, 1.2f, camera.getCameraPosition(), 1.0f, 0.07f, 0.017f, camera.getCameraFront(), 14.0f, 15.5f);


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
		shaderList[0]->setInt("u_Texture", 0);
		
		// Camera movement
		camera.ProcessKeyboard(mainWindow.getKeys(), deltaTime);
		camera.ProcessMouseMovement(mainWindow.getXChange(), mainWindow.getYChange());

		// Set uniform values
		shaderList[0]->setMat4("projection", projection);
		shaderList[0]->setMat4("view", camera.getViewMatrix());
		shaderList[0]->setVec3("cameraPosition", camera.getCameraPosition());

		

		// Set light uniforms
		mainLight->useLight(shaderList[0]);
		pointLight->useLight(shaderList[0]);
		pointLight2->useLight(shaderList[0]);
		pointLight3->useLight(shaderList[0]);

		// Turn on flashlight if enabled
		if (camera.getFlashlightState()) {
			flashlight->setLightPosition(camera.getCameraPosition());
			flashlight->setLightDirection(camera.getCameraFront());
			flashlight->useLight(shaderList[0]);
		}
		else
		{
			shaderList[0]->setFloat("flashLight.ambientIntensity", 0.0f);
			shaderList[0]->setFloat("flashLight.diffuseIntensity", 0.0f);
		}

		rotation += glm::vec3(0.0f, 35.0f * deltaTime, 0.0f);
		

		// Draw doors model
		doorEntity->setRotation(rotation);
		doorEntity->DrawEntity(shaderList[0]);

		// Draw floor model
		floorEntity->DrawEntity(shaderList[0]);


		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}