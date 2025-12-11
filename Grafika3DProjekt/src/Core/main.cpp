#define STB_IMAGE_IMPLEMENTATION
#define NOMINMAX
#include <Windows.h>
#include <stdio.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include "Window.h"
#include "Rendering/Mesh.h"
#include "Rendering/Shader.h"
#include <vector>
#include "Camera.h"
#include "Entities/Entity.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/Flashlight.h"
#include "Rendering/Texture.h"
#include <assimp/Importer.hpp>
#include "Rendering/Model.h"
#include "Scene.h"
#include "Light/ShadowMap.h"
#include "Rendering/Skybox.h"
#include "Player.h"
#include "Systems/TextRenderer.h"
#include "Entities/Door.h"
#include "UI/UI.h"
#include "Config.h"
#include "Systems/AudioManager.h"
#include "Entities/Key.h"
#include "Systems/SpriteRenderer.h"
#include "Entities/Radio.h"
#include "Entities/Book.h"
#include "Entities/BookshelfPuzzle.h"

enum ShaderTypes
{
	SHADER_DEFAULT,
	SHADER_DIRLIGHT_SHADOWMAP,
	SHADER_OMNI_SHADOWMAP,
	SHADER_OUTLINE,
	SHADER_SPRITES
};

enum GameStates
{
	STATE_MAIN_MENU,
	STATE_PLAYING,
	STATE_PAUSED,
	STATE_GAME_END
};

Config& config = Config::getInstance();

// Window dimensions
 GLint WIDTH = config.screenWidth, HEIGHT = config.screenHeight;

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
static const char* spriteVertexShader = "Shaders/spriteShader.vert";
static const char* spriteFragmentShader = "Shaders/spriteShader.frag";
// Texture file paths
static const char* brickTexture = "Textures/brick.png";
static const char* stoneTexture = "Textures/stone.png";



// Game state
GameStates gameState = STATE_PLAYING;

// Delta time
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);

// UI
UI* gameUI;

// Entity
Door* doorEntity;
Entity* xwingEntity;
Entity* sculptureEntity;
Entity* flashlightEntity;
Entity* framuga;
Entity* paintingEntity;
Entity* keyEntity;
Entity* radioEntity;

// Room 1 walls and floor
Entity* floorRoom1Entity;
Entity* rightWallRoom1Entity;
Entity* leftWallRoom1Entity;
Entity* backWallRoom1Entity;
Entity* doorWallRoom1RightEntity;
Entity* doorWallRoom1LeftEntity;
Entity* doorWallRoom1UpEntity;
Door* doorsRoom1Entity;

// Room 1 interior objects
BookshelfPuzzle* bookshelfEntity;
Book* brownBookEntity;
Book* greenBookEntity;
Book* orangeBookEntity;
Book* purpleBookEntity;
Book* blueBookEntity;
Book* redBookEntity;
Book* yellowBookEntity;
Book* greyBookEntity;

// Light source
DirectionalLight* mainLight;
PointLight* pointLight;
PointLight* pointLight2;
PointLight* pointLight3;
Flashlight* flashlight;

// Create models
Model door;
Model sculpture;
Model flashlightModel;
Model framugaModel;
Model paintingModel;
Model keyModel;
Model radioModel;

// Room 1 walls and floor models
Model floorRoom1Model;
Model fullWallRoom1Model;
Model doorWallRoom1RightModel;
Model doorWallRoom1LeftModel;
Model doorWallRoom1UpModel;
Model doorsRoom1Model;

// Room 1 interior object models
Model bookshelfModel;
Model brownBookModel;
Model greenBookModel;
Model orangeBookModel;
Model purpleBookModel;
Model blueBookModel;
Model redBookModel;
Model yellowBookModel;
Model greyBookModel;


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

// Sprite Renderer
SpriteRenderer* spriteRenderer;


// Sprites
Texture* keySprite;
Texture* itemFrame;
Texture* selectedItemFrame;
Texture* flashlightSprite;

// Audio Manager
AudioManager& audioManager = AudioManager::GetInstance();

// Function prototypes
Scene* createMainScene(Camera* camera);
void DirectionalLightShadowMapPass();
void FlashlightShadowMapPass();
void OmniShadowMapPass(PointLight* pLight);
void RenderScenePass(glm::mat4 projection);
void HandleKeyboardInput(float deltaTime, Scene* currentScene);
void SetGameState(GameStates newState);
void DrawInventory();

int uiWidth, uiHeight;

int main()
{	

	// Create Window
	mainWindow = Window(WIDTH, HEIGHT, config.fullscreen);
	mainWindow.Initialise();

	uiWidth = mainWindow.getBufferWidth();
	uiHeight = mainWindow.getBufferHeight();

	// Create UI
	gameUI = new UI(mainWindow.getWindow());

	UI::wasPauseMenuOpen = false;
	UI::isPauseMenuOpen = false;

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

	Shader* spriteShader = new Shader();
	spriteShader->CreateShader(spriteVertexShader, spriteFragmentShader);
	shaderList.push_back(spriteShader);

	keySprite = new Texture("Textures/Icons/door_key.png");
	keySprite->LoadTextureAlpha();

	itemFrame = new Texture("Textures/Icons/item_frame.png");
	itemFrame->LoadTextureAlpha();
	selectedItemFrame = new Texture("Textures/Icons/item_frame_selected.png");
	selectedItemFrame->LoadTextureAlpha();

	flashlightSprite = new Texture("Textures/Icons/flashlight.png");
	flashlightSprite->LoadTextureAlpha();

	spriteRenderer = new SpriteRenderer(*shaderList[SHADER_SPRITES]);
	glm::mat4 projectionUI = glm::ortho(0.0f, (float)uiWidth, (float)uiHeight, 0.0f, -1.0f, 1.0f);

	shaderList[SHADER_SPRITES]->UseShader();
	shaderList[SHADER_SPRITES]->setInt("image", 0);
	shaderList[SHADER_SPRITES]->setMat4("projection", projectionUI);

	
	// Set perspective 
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / (GLfloat)mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Create main scene
	scene = createMainScene(&camera);

	audioManager.Init();
	audioManager.LoadMusicTrack("background", "Audio/background_music.mp3");
	int backgroundMusicHandle = audioManager.PlayMusicTrack("background", config.musicVolume, 1);
	
	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		float fps = 1.0f / deltaTime;

		audioManager.SetListenerPosition(camera.getCameraPosition());
		audioManager.Update3DAudio();

		// Update audio volumes based on config
		audioManager.UpdateMusicVolume(backgroundMusicHandle, config.musicVolume);
		audioManager.UpdateSFXVolume(config.sfxVolume);

		// Disable huge delta time while loading assets
		if (deltaTime > 0.5f)
		{
			deltaTime = 0.5f;
		}

		if (gameState == STATE_PLAYING) {
			// Camera movement
			camera.ProcessMouseMovement(mainWindow.getXChange(), mainWindow.getYChange());
			// Update the scene
			scene->Update(deltaTime);
			// Update held entity model
			player->setHeldEntityModel(player->getInventory()->GetCurrentItem()->itemModel);
			player->setHeldEntityScale(player->getInventory()->GetCurrentItem()->itemScale);
		}
		
		// Keyboard movement
		HandleKeyboardInput(deltaTime, scene);
		
		// Get + Handle user input events
		glfwPollEvents();
		
		// Shadow map for dirlight pass
		DirectionalLightShadowMapPass();

		// Shadow map for flashlight pass
		FlashlightShadowMapPass();

		// Shadow map for point lights
		for (int i = 0; i < scene->getPointLights().size(); i++)
		{
			OmniShadowMapPass(scene->getPointLights()[i]);
		}

		// Render scene pass
		RenderScenePass(projection);

		// Draw UI
		DrawInventory();

		// Render FPS
		if (config.showFPS) {
			textRenderer->RenderText("FPS: " + std::to_string((int)fps), 10.0f, uiHeight - 20.0f, 1.0f, glm::vec4(0.5f, 0.8f, 0.2f, 1.0f));
		}

		// Render crosshair
		float textWidth = tooltipRenderer->GetTextWidth("+");
		tooltipRenderer->RenderText("+", (mainWindow.getBufferWidth() / 2.0f) - textWidth, (mainWindow.getBufferHeight() / 2.0f) - 10.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		if(gameState == STATE_PAUSED) {
			UI::isPauseMenuOpen = true;
			gameUI->DrawPauseMenu();
		}
		else {
			UI::isPauseMenuOpen = false;
		}

		if(UI::wasPauseMenuOpen && !UI::isPauseMenuOpen) {
			printf("config saved\n");
			config.Save();
			UI::wasPauseMenuOpen = false;
		}

		// Swap buffers
		mainWindow.swapBuffers();
	}

	return 0;
}

Scene* createMainScene(Camera * camera) {



	// Load Models
	door.LoadModel("Models/door.obj");
	flashlightModel.LoadModel("Models/flashlight.obj");
	framugaModel.LoadModel("Models/framuga.obj");
	paintingModel.LoadModel("Models/V3TEST.obj");
	keyModel.LoadModel("Models/Worn_Key.obj");
	radioModel.LoadModel("Models/radio.obj");

	// Room 1 walls and floor models
	floorRoom1Model.LoadModel("Models/floorRoom1.obj");
	fullWallRoom1Model.LoadModel("Models/fullWallRoom1.obj");
	doorWallRoom1RightModel.LoadModel("Models/doorWallRoom1Right.obj");
	doorWallRoom1LeftModel.LoadModel("Models/doorWallRoom1Left.obj");
	doorWallRoom1UpModel.LoadModel("Models/doorWallRoom1Up.obj");
	doorsRoom1Model.LoadModel("Models/exitDoorsRoom1.obj");

	// Room 1 interior object models
	bookshelfModel.LoadModel("Models/bookshelf.obj");
	brownBookModel.LoadModel("Models/book.obj");
	greenBookModel.LoadModel("Models/greenBook.obj");
	orangeBookModel.LoadModel("Models/orangeBook.obj");
	purpleBookModel.LoadModel("Models/purpleBook.obj");
	blueBookModel.LoadModel("Models/blueBook.obj");
	redBookModel.LoadModel("Models/redBook.obj");
	yellowBookModel.LoadModel("Models/yellowBook.obj");
	greyBookModel.LoadModel("Models/greyBook.obj");

	/*sculpture.LoadModel("Models/rzezba.obj");*/


	// Create Entities
	framuga = new Entity(&framugaModel,glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.41f));
	doorEntity = new Door(&door, glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(1.4f), "Doors", framuga, "mainKey");
	doorEntity->setLocked(true);
	flashlightEntity = new Entity(&flashlightModel, glm::vec3(5.0f,2.0f,-3.0f), glm::vec3(0.0f), glm::vec3(0.03f));
	paintingEntity = new Entity(&paintingModel,  glm::vec3(-4.0f, 2.0f, -8.0f), glm::vec3(180.0f, 90.0f, 90.0f), glm::vec3(2.0f), true);
	paintingEntity->setTitle("Mieszko I");
	flashlightEntity->setCastsShadow(false);
	flashlightEntity->setTitle("Flashlight");
	keyEntity = new Key(&keyModel, glm::vec3(1.0f, 1.0f, -2.0f), glm::vec3(90.0f,0.0f,0.0f), glm::vec3(0.75f), "mainKey", keySprite, true);
	keyEntity->setTitle("Key");
	keyEntity->setColissions(false);
	radioEntity = new Radio(&radioModel, glm::vec3(-4.0f, 0.0f, -3.0f), glm::vec3(0.0f), glm::vec3(4.0f), true);
	radioEntity->setTitle("Radio");

	floorRoom1Entity = new Entity(&floorRoom1Model, glm::vec3(2.5f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(1.5f));
	rightWallRoom1Entity = new Entity(&fullWallRoom1Model, glm::vec3(2.0f, -0.0f, -8.0f), glm::vec3(0.0f), glm::vec3(1.8f));
	backWallRoom1Entity = new Entity(&fullWallRoom1Model, glm::vec3(-6.5f, -0.0f, -8.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(1.8f));
	leftWallRoom1Entity = new Entity(&fullWallRoom1Model, glm::vec3(-6.0f, -0.0f, 1.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(1.8f));
	doorWallRoom1RightEntity = new Entity(&doorWallRoom1RightModel, glm::vec3(-7.0f, -0.10, 1.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.8f));
	doorWallRoom1LeftEntity = new Entity(&doorWallRoom1LeftModel, glm::vec3(-7.0f, -0.0f, 1.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.8f));	
	doorWallRoom1UpEntity = new Entity(&doorWallRoom1UpModel, glm::vec3(-7.0f, -0.0f, 1.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.8f));
	doorsRoom1Entity = new Door(&doorsRoom1Model, glm::vec3(-4.0f, 0.0f, 1.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(1.8f), "Doors", framuga, "finalExitKey");
	doorsRoom1Entity->setLocked(false);

	bookshelfEntity = new BookshelfPuzzle(&bookshelfModel, glm::vec3(1.0f, 0.0f, -7.5f), glm::vec3(0.0f), glm::vec3(1.5f));

	redBookEntity = new Book(&redBookModel, glm::vec3(0.42f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "redBook", true);
	redBookEntity->setTitle("Red Book");
	redBookEntity->setColissions(false);
	greyBookEntity = new Book(&greyBookModel, glm::vec3(0.48f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "greyBook", true);
	greyBookEntity->setTitle("Grey Book");
	greyBookEntity->setColissions(false);
	yellowBookEntity = new Book(&yellowBookModel, glm::vec3(0.54f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "yellowBook", true);
	yellowBookEntity->setTitle("Yellow Book");
	yellowBookEntity->setColissions(false);
	brownBookEntity = new Book(&brownBookModel, glm::vec3(0.6f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "brownBook", true);
	brownBookEntity->setTitle("Brown Book");
	brownBookEntity->setColissions(false);
	greenBookEntity = new Book(&greenBookModel, glm::vec3(0.66f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "greenBook", true);
	greenBookEntity->setTitle("Green Book");
	greenBookEntity->setColissions(false);
	orangeBookEntity = new Book(&orangeBookModel, glm::vec3(0.72f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "orangeBook", true);
	orangeBookEntity->setTitle("Orange Book");
	orangeBookEntity->setColissions(false);
	purpleBookEntity = new Book(&purpleBookModel, glm::vec3(0.78f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "purpleBook", true);
	purpleBookEntity->setTitle("Purple Book");
	purpleBookEntity->setColissions(false);
	blueBookEntity = new Book(&blueBookModel, glm::vec3(0.84f, 1.54f, -7.3f), glm::vec3(0.0f, 0.0f, 90.0f), glm::vec3(1.4f), "blueBook", true);
	blueBookEntity->setTitle("Blue Book");
	blueBookEntity->setColissions(false);



	/*sculptureEntity = new Entity(&sculpture, lessShinyMaterial, glm::vec3(-10.0f, -1.0f, -4.0f), glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(4.0f));
	sculptureEntity->setTitle("Sculpture");*/

	// Create Player
	player = new Player(camera, flashlightEntity);

	// Create flashlight item
	Item flashlightItem;
	flashlightItem.tag = "flashlight";
	flashlightItem.title = "Flashlight";
	flashlightItem.imageTexture = flashlightSprite;
	flashlightItem.itemModel = &flashlightModel;
	player->getInventory()->AddItem(flashlightItem.tag, flashlightItem.title, flashlightItem.imageTexture, flashlightItem.itemModel, flashlightEntity->getScale());

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
	flashlight = new Flashlight(glm::vec3(1.0f, 1.0f, 0.85f), 0.001f, 1.2f, camera->getCameraPosition(), 1.0f, 0.07f, 0.017f, camera->getCameraFront(), 25.0f, 32.5f, 2048.0f,2048.0f);

	// Create scene
	scene = new Scene(camera, player, tooltipRenderer);

	// Add entities and lights to scene
	scene->AddPointLight(pointLight);
	scene->AddPointLight(pointLight2);
	scene->AddPointLight(pointLight3);
	scene->SetDirectionalLight(mainLight);
	scene->SetFlashlight(flashlight);

	scene->AddEntity(floorRoom1Entity);
	scene->AddEntity(rightWallRoom1Entity);
	scene->AddEntity(backWallRoom1Entity);
	scene->AddEntity(leftWallRoom1Entity);
	scene->AddEntity(doorWallRoom1RightEntity);
	scene->AddEntity(doorWallRoom1LeftEntity);
	scene->AddEntity(doorWallRoom1UpEntity);
	scene->AddEntity(doorsRoom1Entity);
	scene->AddEntity(bookshelfEntity);
	scene->AddEntity(brownBookEntity);
	scene->AddEntity(greenBookEntity);
	scene->AddEntity(orangeBookEntity);
	scene->AddEntity(purpleBookEntity);
	scene->AddEntity(blueBookEntity);
	scene->AddEntity(redBookEntity);
	scene->AddEntity(yellowBookEntity);
	scene->AddEntity(greyBookEntity);



	/*scene->AddEntity(doorEntity);*/
	scene->AddEntity(flashlightEntity);
	/*scene->AddEntity(framuga);*/
	scene->AddEntity(paintingEntity);
	//scene->AddEntity(keyEntity);
	scene->AddEntity(radioEntity);


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
	scene->RenderWithOutline(shaderList[SHADER_DEFAULT], projectionMatrix, uiWidth, uiHeight);

	// Render outlines
	glEnable(GL_DEPTH_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilMask(0x00);
	glCullFace(GL_FRONT);

	shaderList[SHADER_OUTLINE]->UseShader();
	float outline = 0.0075f;
	shaderList[SHADER_OUTLINE]->setFloat("outline", outline);
	shaderList[SHADER_OUTLINE]->setVec3("outlineColor", glm::vec3(config.outlineColor[0],config.outlineColor[1],config.outlineColor[2]));
	scene->RenderWithOutline(shaderList[SHADER_OUTLINE], projectionMatrix, uiWidth, uiHeight);


	// Render flashlight last (to be in front of all objects)
	glDepthMask(GL_TRUE);
	glStencilMask(0xFF);
	glCullFace(GL_BACK);
	glDisable(GL_STENCIL_TEST);
	if (player->getHeldEntity()) {
		scene->RenderHeldEntity(shaderList[SHADER_DEFAULT], projectionMatrix);
	}
}

void HandleKeyboardInput(float deltaTime, Scene* currentScene) {

	if (mainWindow.getKeys()[GLFW_KEY_ESCAPE])
	{
		if (gameState == STATE_PLAYING)
		{
			SetGameState(STATE_PAUSED);
		}
		else if (gameState == STATE_PAUSED)
		{
			SetGameState(STATE_PLAYING);
		}
		mainWindow.getKeys()[GLFW_KEY_ESCAPE] = false;
	}
	
	if(gameState != STATE_PLAYING) {
		return;
	}

	float speed = camera.getMovementSpeed();
	if (player->getCrouching())
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
	player->setVelocityX(0.0f);
	player->setVelocityZ(0.0f);
	if (mainWindow.getKeys()[GLFW_KEY_W])
	{
		player->setVelocity(player->getVelocity() += front * speed);
		isMoving = true;
	}
	if (mainWindow.getKeys()[GLFW_KEY_S])
	{
		player->setVelocity(player->getVelocity() -= front * speed);
		isMoving = true;
	}
	if (mainWindow.getKeys()[GLFW_KEY_A])
	{
		player->setVelocity(player->getVelocity() -= right * speed);
		isMoving = true;
	}
	if (mainWindow.getKeys()[GLFW_KEY_D])
	{
		player->setVelocity(player->getVelocity() += right * speed);
		isMoving = true;
	}

	if (mainWindow.getKeys()[GLFW_KEY_SPACE])
	{
		player->Jump();
	}

	if (mainWindow.getKeys()[GLFW_KEY_F])
	{
		if(player->getInventory()->GetCurrentItem()->tag != "flashlight") {
			return;
		}
		if (player->getFlashlightState())
			player->changeFlashlightState(false);
		else
			player->changeFlashlightState(true);
		mainWindow.getKeys()[GLFW_KEY_F] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_E])
	{
		mainWindow.getKeys()[GLFW_KEY_E] = false;

		Entity* target = player->getTargettedEntity();
		if (!target) return;
		if (!target->getInteractable()) return;
		target->Interact(player->getInventory());
		if (!target->getPickable()) return;
		player->pickUpEntity(target);
		return;
	}
	if (mainWindow.getKeys()[GLFW_KEY_LEFT_SHIFT])
	{
		player->Crouch(true);
	}
	else {
		player->Crouch(false);
	}
	if (isMoving) {
		player->setWalkTimer(player->getWalkTimer() + deltaTime);
	}
	else {
		// TODO smooth reset
		player->setWalkTimer(0.0f);
	}

	if (mainWindow.getKeys()[GLFW_KEY_1]) {
		player->getInventory()->SetCurrentItem(0);
		mainWindow.getKeys()[GLFW_KEY_1] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_2]) {
		player->getInventory()->SetCurrentItem(1);
		mainWindow.getKeys()[GLFW_KEY_2] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_3]) {
		player->getInventory()->SetCurrentItem(2);
		mainWindow.getKeys()[GLFW_KEY_3] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_4]) {
		player->getInventory()->SetCurrentItem(3);
		mainWindow.getKeys()[GLFW_KEY_4] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_5]) {
		player->getInventory()->SetCurrentItem(4);
		mainWindow.getKeys()[GLFW_KEY_5] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_6]) {
		player->getInventory()->SetCurrentItem(5);
		mainWindow.getKeys()[GLFW_KEY_6] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_7]) {
		player->getInventory()->SetCurrentItem(6);
		mainWindow.getKeys()[GLFW_KEY_7] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_8]) {
		player->getInventory()->SetCurrentItem(7);
		mainWindow.getKeys()[GLFW_KEY_8] = false;
	}
	if (mainWindow.getKeys()[GLFW_KEY_9]) {
		player->getInventory()->SetCurrentItem(8);
		mainWindow.getKeys()[GLFW_KEY_9] = false;
	}


	double currentScrollY = mainWindow.getScrollY();
	if (currentScrollY != 0.0) {
		if (currentScrollY > 0.0) {
			player->getInventory()->ChangeCurrentItem(1);
		}
		else {
			player->getInventory()->ChangeCurrentItem(-1);
		}
	}

}

void SetGameState(GameStates newState) {
	gameState = newState;
	if(gameState == STATE_PLAYING) {
		glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	} else {
		glfwSetInputMode(mainWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}

void DrawInventory() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	glDepthMask(GL_FALSE);
	std::vector<Item> inventory = player->getInventory()->GetItems();
	float startingX = 30.0f ;
	float offsetY = uiHeight - 100.0f;
	float imageSize = 64.0 ;
	float spacing = imageSize + 15.0f;

	for(int i = 0 ; i < player->getInventory()->GetMaxItems(); i++) {
		float spritePosX = startingX + i * spacing;
		float spritePosY = offsetY;
		if(i == player->getInventory()->GetCurrentItemIndex()) {
			spriteRenderer->DrawSprite(selectedItemFrame, glm::vec2(spritePosX,spritePosY), glm::vec2(imageSize, imageSize));
		}
		else {
			spriteRenderer->DrawSprite(itemFrame, glm::vec2(spritePosX, spritePosY), glm::vec2(imageSize, imageSize));
		}
	}

	int i = 0;
	for (auto &item : inventory) {
		float spritePosX = startingX + i * spacing;
		float spritePosY = offsetY;
		spriteRenderer->DrawSprite(item.imageTexture, glm::vec2(spritePosX,spritePosY), glm::vec2(imageSize, imageSize));
		i++;
	}
	i = 0;
	for (auto & item : inventory) {
		float spritePosX = startingX + i * spacing;
		float spriteCenterX = spritePosX + imageSize / 2.0f;
		float textWidth = textRenderer->GetTextWidth(item.title);
		float textStartX = spriteCenterX - (textWidth / 2.0f);
		float textPosY = uiHeight - offsetY - imageSize - 15.0f;
		textRenderer->RenderText(item.title, textStartX, textPosY, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		i++;
	}
	player->getInventory()->DrawNotification(deltaTime);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}