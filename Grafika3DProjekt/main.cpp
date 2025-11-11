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
#include "SpinningEntity.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "PointLight.h"
#include "Flashlight.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include "Model.h"
#include "Scene.h"

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

// Texture file paths
static const char* brickTexture = "Textures/brick.png";
static const char* stoneTexture = "Textures/stone.png";

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

// Entity
SpinningEntity* doorEntity;
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

// Create scene
Scene* scene = nullptr;

Scene* createMainScene(Camera* camera);

int main()
{
	// Create Window
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	// Create Shaders
	Shader* shader1 = new Shader();
	shader1->CreateShader(vertexShader, fragmentShader);
	shaderList.push_back(shader1);

	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Create main scene
	scene = createMainScene(&camera);

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

		// Camera movement
		camera.ProcessKeyboard(mainWindow.getKeys(), deltaTime);
		camera.ProcessMouseMovement(mainWindow.getXChange(), mainWindow.getYChange());
	
		// Update scene
		scene->Update(deltaTime);
		scene->Render(shaderList[0], projection);

		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}

Scene* createMainScene(Camera * camera) {

	// Create scene
	scene = new Scene(camera);

	// Load Models
	door.LoadModel("Models/door.obj");
	floorModel.LoadModel("Models/Cranberry_Doormat.obj");

	// Create Materials
	shinyMaterial = new Material(1.0f, 64.0f);
	lessShinyMaterial = new Material(0.5f, 128.0f);

	// Create Entities
	doorEntity = new SpinningEntity(&door, shinyMaterial, glm::vec3(0.0f, -1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.8f));
	floorEntity = new Entity(&floorModel, lessShinyMaterial, glm::vec3(0.0f, -1.5f, -3.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	scene->AddEntity(doorEntity);
	scene->AddEntity(floorEntity);

	// Light
	mainLight = new DirectionalLight(glm::vec3(0.0f, 0.5f, 1.0f), glm::vec3(2.0f, -1.0f, -4.0f), 0.15f, 0.8f);
	pointLight = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.5f, 0.9f, glm::vec3(0.0f, 1.0f, -3.5f), 1.0f, 0.09f, 0.032f, 0);
	pointLight2 = new PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.5f, 0.9f, glm::vec3(-3.5f, 0.5f, -4.0f), 1.0f, 0.12f, 0.062f, 1);
	pointLight3 = new PointLight(glm::vec3(0.0f, 1.0f, 0.0f), 0.5f, 0.9f, glm::vec3(3.5f, 0.5f, -4.0f), 1.0f, 0.12f, 0.062f, 2);
	flashlight = new Flashlight(glm::vec3(1.0f, 1.0f, 0.85f), 0.02f, 1.2f, camera->getCameraPosition(), 1.0f, 0.07f, 0.017f, camera->getCameraFront(), 14.0f, 15.5f);

	// Add entities and lights to scene
	scene->AddEntity(doorEntity);
	scene->AddEntity(floorEntity);
	scene->AddPointLight(pointLight);
	scene->AddPointLight(pointLight2);
	scene->AddPointLight(pointLight3);
	scene->SetDirectionalLight(mainLight);
	scene->SetFlashlight(flashlight);

	return scene;
}