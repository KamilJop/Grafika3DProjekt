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
#include "Scene.h"
#include "ShadowMap.h"
#include "Skybox.h"
#include "Player.h"
#include "TextRenderer.h"
#include "Door.h"

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
static const char* omniShadowVertexShader = "Shaders/omni_shadow_map.vert";
static const char* omniShadowGeometryShader = "Shaders/omni_shadow_map.geom";
static const char* omniShadowFragmentShader = "Shaders/omni_shadow_map.frag";
static const char* outlineVertexShader = "Shaders/outline.vert";
static const char* outlineFragmentShader = "Shaders/outline.frag";
// Texture file paths
static const char* brickTexture = "Textures/brick.png";
static const char* stoneTexture = "Textures/stone.png";

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

// Entity
Door* doorEntity;
Entity* floorEntity;
Entity* xwingEntity;
Entity* chestEntity;
Entity* sculptureEntity;
Entity* testWallEntity;
Entity* flashlightEntity;
Entity* framuga;
Entity* paintingEntity;

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
Model sculpture;
Model testWall;
Model flashlightModel;
Model framugaModel;
Model paintingModel;

// Create player
Player* player;

// Create scene
Scene* scene = nullptr;

// Create skybox
Skybox* skybox;

// Skybox faces
std::vector<std::string> skyboxFaces
{
	"Textures/Skybox/Cold Night__Cam_2_Left+X.png",
	"Textures/Skybox/Cold Night__Cam_3_Right-X.png",
	"Textures/Skybox/Cold Night__Cam_4_Up+Y.png",
	"Textures/Skybox/Cold Night__Cam_5_Down-Y.png",
	"Textures/Skybox/Cold Night__Cam_0_Front+Z.png",
	"Textures/Skybox/Cold Night__Cam_1_Back-Z.png"
};

// Text Renderer
TextRenderer* textRenderer;
TextRenderer* tooltipRenderer;


// Function prototypes
Scene* createMainScene(Camera* camera);
void DirectionalLightShadowMapPass();
void FlashlightShadowMapPass();
void OmniShadowMapPass(PointLight* pLight);
void RenderScenePass(glm::mat4 projection);
void HandleKeyboardInput(float deltaTime, Scene* currentScene);


enum ShaderTypes
{
	SHADER_DEFAULT,
	SHADER_DIRLIGHT_SHADOWMAP,
	SHADER_OMNI_SHADOWMAP,
	SHADER_OUTLINE,
};

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

	Shader* omniShadowShader = new Shader();
	omniShadowShader->CreateShader(omniShadowVertexShader, omniShadowGeometryShader, omniShadowFragmentShader);
	shaderList.push_back(omniShadowShader);

	Shader* outlineShader = new Shader();
	outlineShader->CreateShader(outlineVertexShader, outlineFragmentShader);
	shaderList.push_back(outlineShader);

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
		float fps = 1.0f / deltaTime;
		// Disable huge delta time while loading assets
		if (deltaTime > 0.5f)
		{
			deltaTime = 0.5f;
		}

		// Camera movement
		camera.ProcessMouseMovement(mainWindow.getXChange(), mainWindow.getYChange());

		// Keyboard movement
		HandleKeyboardInput(deltaTime, scene);
		
		// Get + Handle user input events
		glfwPollEvents();
		
		// Shadow map for dirlight pass
		DirectionalLightShadowMapPass();

		// Shadow map for flashlight pass
		FlashlightShadowMapPass();

		// Shadow map for point lights
		for (int i = 0; i < scene->pointLights.size(); i++)
		{
			OmniShadowMapPass(scene->pointLights[i]);
		}

		// Update the scene
		scene->Update(deltaTime);

		// Render scene pass
		RenderScenePass(projection);

		// Render FPS
		textRenderer->RenderText("FPS: " + std::to_string((int)fps), 25.0f, 25.0f, 1.0f, glm::vec3(0.5f, 0.8f, 0.2f));

		// Render crosshair
		float textWidth = tooltipRenderer->GetTextWidth("+");
		tooltipRenderer->RenderText("+", (mainWindow.getBufferWidth() / 2.0f) - textWidth, (mainWindow.getBufferHeight() / 2.0f) - 10.0f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}

Scene* createMainScene(Camera * camera) {



	// Load Models
	door.LoadModel("Models/door.obj");
	floorModel.LoadModel("Models/Cranberry_Doormat.obj");
	chest.LoadModel("Models/Untitled.obj");
	testWall.LoadModel("Models/testsciana.obj");
	flashlightModel.LoadModel("Models/flashlight.obj");
	framugaModel.LoadModel("Models/framuga.obj");
	paintingModel.LoadModel("Models/chrobry.obj");

	/*sculpture.LoadModel("Models/rzezba.obj");*/


	// Create Materials
	shinyMaterial = new Material(0.7f, 64.0f);
	lessShinyMaterial = new Material(0.5f, 256.0f);

	// Create Entities
	framuga = new Entity(&framugaModel, lessShinyMaterial, glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.41f));
	doorEntity = new Door(&door, shinyMaterial, glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.4f), "Doors", framuga);
	floorEntity = new Entity(&floorModel, lessShinyMaterial, glm::vec3(0.0f, -0.6f, -3.0f), glm::vec3(0.0f), glm::vec3(0.5f));
	chestEntity = new Entity(&chest, shinyMaterial, glm::vec3(2.0f, 0.5f, -4.0f), glm::vec3(0.0f, -45.0f, 0.0f), glm::vec3(1.3f));
	testWallEntity = new Entity(&testWall, lessShinyMaterial, glm::vec3(-2.0f, -0.5f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f));
	flashlightEntity = new Entity(&flashlightModel, shinyMaterial, glm::vec3(5.0f,2.0f,-3.0f), glm::vec3(0.0f), glm::vec3(0.03f));
	paintingEntity = new Entity(&paintingModel, lessShinyMaterial, glm::vec3(-8.0f, 3.0f, -4.0f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(5.0f), true);
	paintingEntity->setTitle("Boleslaw Chrobry");
	flashlightEntity->setCastsShadow(false);
	flashlightEntity->setTitle("Flashlight");
	
	/*sculptureEntity = new Entity(&sculpture, lessShinyMaterial, glm::vec3(-10.0f, -1.0f, -4.0f), glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(4.0f));
	sculptureEntity->setTitle("Sculpture");*/

	// Create Player
	player = new Player(camera, flashlightEntity);

	// Text renderer
	textRenderer = new TextRenderer(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
	tooltipRenderer = new TextRenderer(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
	// Load font
	textRenderer->Load("Fonts/BitterPro-Medium.ttf", 12);
	tooltipRenderer->Load("Fonts/BitterPro-Bold.ttf", 36);

	// Skybox
	skybox = new Skybox(skyboxFaces);

	// Light
	mainLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -5.0f, -5.5f), 0.3f, 0.22f, 2048.0f, 2048.0f);
	pointLight = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.3f, 0.9f, glm::vec3(-10.0f, 1.0f, -3.0f), 1.0f, 0.09f, 0.032f, 0, 100.0f, 0.01f, 2048.0f, 2048.0f);
	pointLight2 = new PointLight(glm::vec3(0.0f, 0.0f, 1.0f), 0.25f, 0.9f, glm::vec3(8.0f, 1.5f, -6.0f), 1.0f, 0.12f, 0.062f, 1, 100.0f, 0.01f, 2048.0f, 2048.0f);
	pointLight3 = new PointLight(glm::vec3(1.0f, 1.0f, 1.0f), 0.25f, 0.9f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.12f, 0.062f, 2, 100.0f, 0.01f, 2048.0f, 2048.0f);
	flashlight = new Flashlight(glm::vec3(1.0f, 1.0f, 0.85f), 0.001f, 3.2f, camera->getCameraPosition(), 1.0f, 0.07f, 0.017f, camera->getCameraFront(), 25.0f, 32.5f, 2048.0f,2048.0f);

	// Create scene
	scene = new Scene(camera, player, tooltipRenderer);

	// Add entities and lights to scene
	scene->AddPointLight(pointLight);
	scene->AddPointLight(pointLight2);
	scene->AddPointLight(pointLight3);
	scene->SetDirectionalLight(mainLight);
	scene->SetFlashlight(flashlight);

	scene->AddEntity(doorEntity);
	scene->AddEntity(floorEntity);
	scene->AddEntity(chestEntity);
	scene->AddEntity(testWallEntity);
	scene->AddEntity(flashlightEntity);
	scene->AddEntity(framuga);
	scene->AddEntity(paintingEntity);
	/*scene->AddEntity(sculptureEntity);*/

	return scene;
}

void DirectionalLightShadowMapPass() {
	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->UseShader();
	glViewport(0, 0, mainLight->getShadowMap()->getShadowWidth(), mainLight->getShadowMap()->getShadowHeight());
	glCullFace(GL_FRONT);
	mainLight->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lightTransform = mainLight->CalculateLightTransform();
	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->setMat4("lightSpaceTransform", lightTransform);

	scene->RenderShadowMap(shaderList[SHADER_DIRLIGHT_SHADOWMAP]);

	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FlashlightShadowMapPass() {

	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->UseShader();
	glViewport(0, 0, flashlight->getShadowMap()->getShadowWidth(), flashlight->getShadowMap()->getShadowHeight());
	glCullFace(GL_FRONT);
	flashlight->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lightTransform = flashlight->CalculateLightTransform();

	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->setMat4("lightSpaceTransform", lightTransform);

	scene->RenderShadowMap(shaderList[SHADER_DIRLIGHT_SHADOWMAP]);
	glCullFace(GL_BACK);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* pLight) {

	// Use the omni-directional shadow map shader
	shaderList[SHADER_OMNI_SHADOWMAP]->UseShader();
	// Setup viewport
	glViewport(0, 0, pLight->getShadowMap()->getShadowWidth(), pLight->getShadowMap()->getShadowHeight());

	// Turn on front face culling
	glCullFace(GL_FRONT);

	// Bind the shadow map for writing
	pLight->getShadowMap()->Write();

	// Clear depth buffer
	glClear(GL_DEPTH_BUFFER_BIT);

	// Get the light transformation matrices
	std::vector<glm::mat4> lightTransforms = pLight->calculateLightTransform();
	for (GLuint i = 0; i < 6; ++i)
	{
		std::string uniformName = "lightMatrices[" + std::to_string(i) + "]";
		shaderList[SHADER_OMNI_SHADOWMAP]->setMat4(uniformName, lightTransforms[i]);
	}
	shaderList[SHADER_OMNI_SHADOWMAP]->setFloat("farPlane", pLight->getFarPlane());
	shaderList[SHADER_OMNI_SHADOWMAP]->setVec3("lightPos", pLight->getLightPosition());
	scene->RenderShadowMap(shaderList[SHADER_OMNI_SHADOWMAP]);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderScenePass(glm::mat4 projectionMatrix)
{
	// Setup viewport and clear buffers
	glViewport(0, 0, mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	// Draw skybox 
	glDisable(GL_STENCIL_TEST);
	skybox->DrawSkybox(camera.getViewMatrix(), projectionMatrix);
	glEnable(GL_STENCIL_TEST);
	
	// Shader configurations
	shaderList[SHADER_DEFAULT]->UseShader();
	shaderList[SHADER_DEFAULT]->setInt("directionalShadowMap", 1);
	mainLight->getShadowMap()->Read(GL_TEXTURE1);
	shaderList[SHADER_DEFAULT]->setInt("flashShadowMap", 2);
	flashlight->getShadowMap()->Read(GL_TEXTURE2);
	shaderList[SHADER_DEFAULT]->setInt("material.textureMap", 0);
	shaderList[SHADER_DEFAULT]->setInt("material.normalMap", 3);
	shaderList[SHADER_DEFAULT]->setInt("material.heightMap", 4);

	// Render scene without outlines not writing to stencil buffer
	glStencilMask(0x00);
	scene->RenderWithoutOutline(shaderList[SHADER_DEFAULT], projectionMatrix);

	// Now render objects again but only writing to stencil buffer
	glStencilMask(0xFF);
	glStencilFunc(GL_ALWAYS, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	scene->RenderWithOutline(shaderList[SHADER_DEFAULT], projectionMatrix);

	// Render outlines
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glCullFace(GL_FRONT);

	shaderList[SHADER_OUTLINE]->UseShader();
	float outline = 0.005f;
	shaderList[SHADER_OUTLINE]->setFloat("outline", outline);
	scene->RenderWithOutline(shaderList[SHADER_OUTLINE], projectionMatrix);


	// Render flashlight last (to be in front of all objects)
	glDepthMask(GL_TRUE);
	glStencilMask(0xFF);
	glCullFace(GL_BACK);
	glDisable(GL_STENCIL_TEST);
	scene->RenderFlashlightEntity(shaderList[SHADER_DEFAULT], projectionMatrix);
}

void HandleKeyboardInput(float deltaTime, Scene* currentScene) {

	float speed = camera.MovementSpeed;
	if (player->isCrouching)
	{
		speed *= 0.3f;
	}

	glm::vec3 front = camera.getCameraFront();
	front.y = 0.0f;
	front = glm::normalize(front);

	glm::vec3 right = camera.getCameraRight();
	right.y = 0.0f;
	right = glm::normalize(right);

	bool isMoving = false;
	player->velocity.x = 0.0f;
	player->velocity.z = 0.0f;
	if (mainWindow.getKeys()[GLFW_KEY_W])
	{
		player->velocity += front * speed;
		isMoving = true;
	}
	if (mainWindow.getKeys()[GLFW_KEY_S])
	{
		player->velocity -= front * speed;
		isMoving = true;
	}
	if (mainWindow.getKeys()[GLFW_KEY_A])
	{
		player->velocity -= right * speed;
		isMoving = true;
	}
	if (mainWindow.getKeys()[GLFW_KEY_D])
	{
		player->velocity += right * speed;
		isMoving = true;
	}

	if (mainWindow.getKeys()[GLFW_KEY_SPACE])
	{
		player->Jump();
	}

	if (mainWindow.getKeys()[GLFW_KEY_F])
	{
		player->changeFlashlightState();
		mainWindow.getKeys()[GLFW_KEY_F] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_E])
	{
		for(auto & entity : currentScene->entities) {
			if(entity->isOutlined())
				{
				entity->Interact();
				break;
			}
		}
		mainWindow.getKeys()[GLFW_KEY_E] = false;
	}

	if (mainWindow.getKeys()[GLFW_KEY_LEFT_SHIFT])
	{
		player->Crouch(true);
	}
	else {
		player->Crouch(false);
	}
	if (isMoving) {
		player->walkTimer += deltaTime;
	}
	else {
		// TODO smooth reset
		player->walkTimer = 0.0f;
	}
}