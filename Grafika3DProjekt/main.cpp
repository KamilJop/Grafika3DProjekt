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
#include "ShadowMap.h"
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
static const char* shadowVertexShader = "Shaders/directional_shadow_map.vert";
static const char* shadowFragmentShader = "Shaders/directional_shadow_map.frag";

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
Entity* chestEntity;

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
Model chest;

// Create scene
Scene* scene = nullptr;

Scene* createMainScene(Camera* camera);
void DirectionalLightShadowMapPass();
void RenderScenePass(glm::mat4 projection);

int main()
{
	// Create Window
	mainWindow = Window(WIDTH, HEIGHT);
	mainWindow.Initialise();

	// Create Shaders
	Shader* shader1 = new Shader();
	shader1->CreateShader(vertexShader, fragmentShader);
	shaderList.push_back(shader1);

	Shader* shadowShader = new Shader();
	shadowShader->CreateShader(shadowVertexShader, shadowFragmentShader);
	shaderList.push_back(shadowShader);

	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Create main scene
	scene = createMainScene(&camera);

	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Camera movement
		camera.ProcessKeyboard(mainWindow.getKeys(), deltaTime);
		camera.ProcessMouseMovement(mainWindow.getXChange(), mainWindow.getYChange());

		// Get + Handle user input events
		glfwPollEvents();
		
		// Shadow map pass
		DirectionalLightShadowMapPass();

		// Render scene pass
		RenderScenePass(projection);

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
	chest.LoadModel("Models/Untitled.obj");

	// Create Materials
	shinyMaterial = new Material(0.7f, 64.0f);
	lessShinyMaterial = new Material(0.5f, 256.0f);

	// Create Entities
	doorEntity = new SpinningEntity(&door, shinyMaterial, glm::vec3(0.0f, -1.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.8f));
	floorEntity = new Entity(&floorModel, lessShinyMaterial, glm::vec3(0.0f, -1.5f, -3.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	chestEntity = new Entity(&chest, shinyMaterial, glm::vec3(2.0f, -1.0f, -4.0f), glm::vec3(0.0f, -45.0f, 0.0f), glm::vec3(1.3f));
	scene->AddEntity(doorEntity);
	scene->AddEntity(floorEntity);
	scene->AddEntity(chestEntity);

	// Light
	mainLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(2.0f, -5.0f, -3.0f), 0.1f, 0.4f, 2048.0f, 2048.0f);
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


void DirectionalLightShadowMapPass() {
	shaderList[1]->UseShader();
	glViewport(0, 0, mainLight->getShadowMap()->getShadowWidth(), mainLight->getShadowMap()->getShadowHeight());

	mainLight->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lightTransform = mainLight->CalculateLightTransform();
	shaderList[1]->setMat4("directionalLightSpaceTransform", lightTransform);

	scene->RenderShadowMap(shaderList[1]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void RenderScenePass(glm::mat4 projectionMatrix)
{
	// Go back to the default framebuffer
	glViewport(0, 0, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());

	// Clear buffers
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Use shader program
	shaderList[0]->UseShader();

	// Set the shadow map uniform to texture unit 1
	shaderList[0]->setInt("directionalShadowMap", 1);

	// Bind the shadow map to texture unit 1
	mainLight->getShadowMap()->Read(GL_TEXTURE1);

	// Update the scene
	scene->Update(deltaTime);

	// Render the scene
	scene->Render(shaderList[0], projectionMatrix);
}