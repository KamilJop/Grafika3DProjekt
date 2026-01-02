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
#include "Entities/Lock.h"
#include "Entities/Desk.h"
#include "Entities/Candle.h"
#include "Entities/Lighter.h"
#include "Entities/Battery.h"
#include "Entities/Chest.h"
#include "Entities/HauntedEntity.h"
#include "Entities/ClockMovingPart.h"
#include "Systems/CandlePuzzle.h"	
#include "Entities/Pedestal.h"
#include "Entities/Pickable.h"
#include "Systems/PedestalPuzzle.h"
#include "Rendering/HDRBuffer.h"
#include "Systems/ParticleSystem.h"
#include "Entities/Lamp.h"
#include "Entities/Readable.h"


enum ShaderTypes
{
	SHADER_DEFAULT,
	SHADER_DIRLIGHT_SHADOWMAP,
	SHADER_OMNI_SHADOWMAP,
	SHADER_OUTLINE,
	SHADER_SPRITES,
	SHADER_POST_PROCESSING,
	SHADER_BLOOM,
	SHADER_PARTICLE
};

enum GameStates
{
	STATE_MAIN_MENU,
	STATE_PLAYING,
	STATE_PAUSED,
	STATE_GAME_END,
	STATE_MINIGAME,
	STATE_READING
};


// TEST FLOAT
float exposure = 0.05f;

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
static const char* postProcessingVertexShader = "Shaders/hdr.vert";
static const char* postProcessingFragmentShader = "Shaders/hdr.frag";
static const char* bloomFragmentShader = "Shaders/bloom.frag";
static const char* particleVertexShader = "Shaders/particle.vert";
static const char* particleFragmentShader = "Shaders/particle.frag";
// Texture file paths
static const char* brickTexture = "Textures/brick.png";
static const char* stoneTexture = "Textures/stone.png";


glm::vec3 savedCameraPosition;
float savedCameraYaw;
float savedCameraPitch;
Lock* currentActiveLock = nullptr;


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
Entity* keyEntity;
Entity* radioEntity;
Entity* pageEntity;
Pickable* collarEntity;
Pickable* featherEntity;
Pickable* skullEntity;
Pickable* eyeEntity;
Entity* chessboardEntity;
Entity* globeEntity;
Entity* notebookEntity;
Model notebookModel;
Readable* letterEntity;
Model letterModel;
Readable* lesserSalomonEntity;
Model lesserSalomonModel;
Readable* lecternEntity;
Model lecternModel;
Texture* lecternSprite;

Entity* wineCabinetEntity;
Model wineCabinetModel;
Readable* paperEntity;
Model paperModel;
Texture* paperSprite;

// Room 1 walls and floor
Door* doorsRoom1Entity;
Desk* deskEntity;
Entity* sofaEntity;
Lighter* lighterEntity;
Entity* calenderEntity;
ClockMovingPart* clockElementEntity;
Entity* clockEntity;
Readable* currentReadableEntity;

// Room 1 collisions
Entity* floorEntity;
Entity* ceilingEntity;
Entity* room1LeftWallEntity;
Entity* room1RightWallRightEntity;
Entity* room1RightWallLeftEntity;
Entity* room1RightWallUpEntity;
Entity* room1BackWallLeftSideEntity;
Entity* room1BackWallRightSideEntity;
Entity* room1BackWallUpSideEntity;
Entity* room1FrontWallUpEntity;
Entity* room1FrontWallLeftEntity;
Entity* room1FrontWallRightEntity;
Entity* houseWalls;
Entity* houseFloor;
Entity* houseCeiling;

// Hidden room collisions
Entity* hiddenRoomBackWallEntity;
Entity* hiddenRoomLeftWallEntity;
Entity* hiddenRoomCeilingEntity;


// Corridor collisions
Entity* corridorLeftWallEntity;
Entity* corridorRightWallEntity;
Entity* corridorCeilingEntity;


// Second room collisions
Entity* secondRoomLeftWallEntity;
Entity* secondRoomRightWallEntity;
Entity* secondRoomBackWallEntity;
Entity* secondRoomFrontWallLeftEntity;
Entity* secondRoomFrontWallRightEntity;

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
Battery* battery1Entity;
Battery* battery2Entity;
Chest* chestEntity;
Entity* tableEntity;
Entity* posterEntity;
Lamp* lampEntity;
Key* drawerKeyEntity;
Model drawerKeyModel;
Key* secondKeyEntity;
// Hidden room interior objects
Chest* hiddenRoomChestEntity;
Lock* hiddenRoomLockEntity;

// Corridor interior objects
HauntedEntity* hauntedPaintingEntity1;
HauntedEntity* hauntedPaintingEntity2;
HauntedEntity* hauntedPaintingEntity3;
HauntedEntity* hauntedPaintingEntity4;
Pedestal* pedestalEntity1;
Pedestal* pedestalEntity2;
Pedestal* pedestalEntity3;
Pedestal* pedestalEntity4;
Entity* witchesPaintingEntity;
Entity* carpetEntity;
Model carpetModel;


// Second room interior objects
Candle* candleEntity;
Candle* candle2Entity;
Candle* candle3Entity;
Candle* candle4Entity;
Candle* candle5Entity;
HauntedEntity* pentagramEntity;
Door* secondRoomDoorEntity;
Entity* secondRoomDoorFramuga;
Candle* candle6Entity;
Candle* candle7Entity;
Candle* candle8Entity;
Candle* candle9Entity;
Model candleHolderModel;

//Lock
Lock* lockEntity;


// Light source
DirectionalLight* mainLight;
PointLight* candleLight;
PointLight* candleLight2;
PointLight* candleLight3;
PointLight* candleLight4;
PointLight* candleLight5;
PointLight* candleLight6;
PointLight* candleLight7;
PointLight* candleLight8;
PointLight* candleLight9;
PointLight* pedestalLight1;
PointLight* pedestalLight2;
PointLight* pedestalLight3;
PointLight* pedestalLight4;
PointLight* lampLight;
PointLight* outsideLight;
Flashlight* flashlight;

// Create models
Model door;
Model sculpture;
Model flashlightModel;
Model framugaModel;
Model huntModel1;
Model huntModel2;
Model skullsModel1;
Model skullsModel2;
Model ravensModel1;
Model ravensModel2;
Model cultModel1;
Model cultModel2;
Model witchesModel;
Model keyModel;
Model radioModel;
Model pageModel;
Model candleModel;
Model lighterModel;
Model batteryModel;
Model lowerChestModel;
Model upperChestModel;
Model posterModel;
Model clockElementModel;
Model clockModel;
Model pentagramModel;
Model pentagram2Model;
Model pedestalModel;
Model collarModel;
Model featherModel;
Model skullModel;
Model eyeModel;
Model houseWallsModel;
Model houseFloorModel;
Model houseCeilingModel;
Model lampModel;
Model chessboardModel;
Model globeModel;

// Room 1 walls and floor models
Model doorsRoom1Model;
Model colliderWallModel;

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
Model sofaModel;
Model calenderModel;
Model tableModel;

// Lock models
std::vector<Model*> lockRotatingModels;
Model lockBaseModel;
Model lockRotatingModel1;
Model lockRotatingModel2;
Model lockRotatingModel3;
Model lockRotatingModel4;
Model lockMetalPartModel;


// Desk models
std::vector<Model*> deskDrawerModels;
Model deskModel;
Model deskDoorModel;
Model deskDrawerModelTop;
Model deskDrawerModelBottom;
Model deskDrawerModelMiddle;

CandlePuzzle* candlePuzzle;
PedestalPuzzle*	pedestalPuzzle;

// Create player
Player* player;

// Create scene
Scene* scene = nullptr;

// Create skybox
Skybox* skybox;

// HDR Buffer
HDRBuffer* hdrBuffer;

// Particle System
ParticleSystem* fireParticleSystem;

ParticleProperties fireParticleProperties{
	glm::vec3(0.0f, 0.2f, 0.0f),
	glm::vec4(10.0f, 4.0f, 0.4f, 1.0f),
	glm::vec4(1.0f, 0.1f, 0.0f, 0.0f),
	1.0f,
	0.08f,
	0.015f,
	3000.0f,
};

std::vector<Candle*> candles;

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
TextRenderer* smallerTooltipRenderer;
TextRenderer* subtitlesRenderer;

// Sprite Renderer
SpriteRenderer* spriteRenderer;


// Sprites
Texture* keySprite;
Texture* itemFrame;
Texture* selectedItemFrame;
Texture* flashlightSprite;
Texture* lighterSprite;
Texture* batterySprite;
Texture* letterSprite;
Texture* lesserSalomonSprite;
Texture* crowSprite;
Texture* collarSprite;
Texture* skullSprite;
Texture* eyeSprite;

std::vector<std::string> scarySounds = {
	"Audio/horror_sound.mp3",
	"Audio/horror_sound2.mp3",
	"Audio/horror_sound3.mp3",
	"Audio/horror_sound4.mp3",
};

// Audio Manager
AudioManager& audioManager = AudioManager::GetInstance();

// Function prototypes
Scene* createMainScene(Camera* camera);

void DrawMainMenu();
void DirectionalLightShadowMapPass();
void FlashlightShadowMapPass();
void OmniShadowMapPass(PointLight* pLight);
void RenderScenePass(glm::mat4 projection);
void PostProcessingPass();
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

	subtitlesRenderer = new TextRenderer(uiWidth, uiHeight);
	subtitlesRenderer->Load("Fonts/BitterPro-Medium.ttf", 24);


	// Create UI
	gameUI = new UI(mainWindow.getWindow(), subtitlesRenderer);

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

	Shader* postProcessingShader = new Shader();
	postProcessingShader->CreateShader(postProcessingVertexShader, postProcessingFragmentShader);
	shaderList.push_back(postProcessingShader);

	Shader* bloomShader = new Shader();
	bloomShader->CreateShader(postProcessingVertexShader, bloomFragmentShader);
	shaderList.push_back(bloomShader);

	Shader* particleShader = new Shader();
	particleShader->CreateShader(particleVertexShader, particleFragmentShader);
	shaderList.push_back(particleShader);

	hdrBuffer = new HDRBuffer(uiWidth, uiHeight);

	Texture fireParticleTexture("Textures/Particles/fire.png");
	fireParticleTexture.LoadTextureAlpha();
	fireParticleSystem = new ParticleSystem(shaderList[SHADER_PARTICLE], &fireParticleTexture, 800, fireParticleProperties);

	keySprite = new Texture("Textures/Icons/door_key.png");
	keySprite->LoadTextureAlpha();

	lighterSprite = new Texture("Textures/Icons/lighter.png");
	lighterSprite->LoadTextureAlpha();

	batterySprite = new Texture("Textures/Icons/battery.png");
	batterySprite->LoadTextureAlpha();

	itemFrame = new Texture("Textures/Icons/item_frame.png");
	itemFrame->LoadTextureAlpha();
	selectedItemFrame = new Texture("Textures/Icons/item_frame_selected.png");
	selectedItemFrame->LoadTextureAlpha();

	flashlightSprite = new Texture("Textures/Icons/flashlight.png");
	flashlightSprite->LoadTextureAlpha();

	paperSprite = new Texture("Textures/timeisthekey.png");
	paperSprite->LoadTextureAlpha();

	letterSprite = new Texture("Textures/letter.png");
	letterSprite->LoadTextureAlpha();

	lesserSalomonSprite = new Texture("Textures/lesserSalomon.png");
	lesserSalomonSprite->LoadTextureAlpha();

	lecternSprite = new Texture("Textures/lectern.png");
	lecternSprite->LoadTextureAlpha();

	crowSprite = new Texture("Textures/Icons/crow.png");
	crowSprite->LoadTextureAlpha();

	collarSprite = new Texture("Textures/Icons/collar.png");
	collarSprite->LoadTextureAlpha();

	skullSprite = new Texture("Textures/Icons/skull.png");
	skullSprite->LoadTextureAlpha();

	eyeSprite = new Texture("Textures/Icons/eye.png");
	eyeSprite->LoadTextureAlpha();

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

	for(auto & sound : scarySounds) {
		audioManager.Load2DSoundEffect(sound, sound);
	}
	
	float fpsTimer = 0.0f;
	float fpsAccumulator = 0.0f;
	int frameCount = 0;
	int displayFPS = 0;
	int currentScarySoundIndex = 0;
	float scarySoundTimer = 0.0f;
	float scarySoundInterval = 70.0f; 
	// Loop until window closed
	while (!mainWindow.getShouldClose())
	{
		// Calculate delta time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		fpsAccumulator += deltaTime;

		frameCount++;
		if (fpsAccumulator >= 0.5f) {
			displayFPS = (int)(frameCount / fpsAccumulator);
			fpsAccumulator = 0.0f;
			frameCount = 0;
		}


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

			// Update puzzles
			candlePuzzle->Update(deltaTime);
			pedestalPuzzle->Update();

			// Update particle systems
			fireParticleSystem->Update(deltaTime);
			for (auto& candle : candles) {
				if (candle->getIsLit()) {
					/*fireParticleSystem->SpawnParticles(candle->getPosition() + glm::vec3(-0.1f, 0.4f, -0.1f), 1, glm::vec3(0.1f, 0.0f, 0.1f));*/
					fireParticleSystem->SpawnParticles(candle->getLightPosition() - glm::vec3(0.0f,0.1f,0.0f), 1, glm::vec3(0.1f, 0.0f, 0.1f));
				}
			}

			if(pedestalPuzzle->getSolved()) {
				lampLight->setAmbientIntensity(0.0f);
				lampLight->setDiffuseIntensity(0.0f);
				pedestalLight1->setAmbientIntensity(0.0f);
				pedestalLight1->setDiffuseIntensity(0.0f);
				pedestalLight2->setAmbientIntensity(0.0f);
				pedestalLight2->setDiffuseIntensity(0.0f);
				pedestalLight3->setAmbientIntensity(0.0f);
				pedestalLight3->setDiffuseIntensity(0.0f);
				pedestalLight4->setAmbientIntensity(0.0f);
				pedestalLight4->setDiffuseIntensity(0.0f);
				outsideLight->setAmbientIntensity(1.0f);
				outsideLight->setDiffuseIntensity(1.0f);
				outsideLight->setColor(glm::vec3(100.0f, 100.0f, 100.0f));
				exposure += 0.1f * deltaTime;
				if(exposure > 3.0f) {
					exposure = 1.5f;
				}

			}
			scarySoundTimer += deltaTime;
			if (scarySoundTimer >= scarySoundInterval) {
				audioManager.Play2DSoundEffect(scarySounds[currentScarySoundIndex], config.sfxVolume * 2.0f);
				printf("Playing scary sound: %s\n", scarySounds[currentScarySoundIndex].c_str());
				currentScarySoundIndex = (currentScarySoundIndex + 1) % scarySounds.size();
				scarySoundTimer = 0.0f;

			}

		}

		if (gameState == STATE_MINIGAME && currentActiveLock != nullptr) {
			if(currentActiveLock->getIsUnlocked()) {
				// Restore camera position
				camera.setPosition(savedCameraPosition);
				camera.setYaw(savedCameraYaw);
				camera.setPitch(savedCameraPitch);
				// Close lock minigame and delete lock
				currentActiveLock->cleanUpLock();
				currentActiveLock = nullptr;
				// Change game state back to playing
				SetGameState(STATE_PLAYING);
			}
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
			if (scene->getPointLights()[i]->getCastShadow()) {
				OmniShadowMapPass(scene->getPointLights()[i]);
			}
		}

		// Render scene pass
		RenderScenePass(projection);

		// Post processing pass
		PostProcessingPass();

		// Draw UI

		gameUI->RenderSubtitle(deltaTime);
		DrawInventory();



		// Render FPS
		if (config.showFPS) {
			textRenderer->RenderText("x " + std::to_string(camera.getCameraPosition().x) + "y "+std::to_string(camera.getCameraPosition().y) + "z" + std::to_string(camera.getCameraPosition().z) + "FPS: " + std::to_string((int)displayFPS), 10.0f, uiHeight - 20.0f, 1.0f, glm::vec4(0.5f, 0.8f, 0.2f, 1.0f));
		}

		if (gameState == STATE_PLAYING) {
			// Render crosshair
			float textWidth = tooltipRenderer->GetTextWidth("+");
			tooltipRenderer->RenderText("+", (mainWindow.getBufferWidth() / 2.0f) - textWidth, (mainWindow.getBufferHeight() / 2.0f) - 10.0f, 1.0f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		}


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
	huntModel1.LoadModel("Models/paintingHunt.obj");
	huntModel2.LoadModel("Models/paintingHuntCursed.obj");
	skullsModel1.LoadModel("Models/paintingSkulls.obj");
	skullsModel2.LoadModel("Models/paintingSkullsCursed.obj");
	ravensModel1.LoadModel("Models/paintingRavens.obj");
	ravensModel2.LoadModel("Models/paintingRavensCursed.obj");
	cultModel1.LoadModel("Models/paintingCult.obj");
	cultModel2.LoadModel("Models/paintingCultCursed.obj");
	witchesModel.LoadModel("Models/paintingWitches.obj");
	keyModel.LoadModel("Models/Worn_Key.obj");
	radioModel.LoadModel("Models/radio.obj");
	posterModel.LoadModel("Models/poster.obj");
	clockElementModel.LoadModel("Models/clockPart.obj");
	clockModel.LoadModel("Models/clock.obj");
	collarModel.LoadModel("Models/collar.obj");
	featherModel.LoadModel("Models/feather.obj");
	skullModel.LoadModel("Models/skull.obj");
	eyeModel.LoadModel("Models/eye1.obj");
	lampModel.LoadModel("Models/lamp.obj");
	chessboardModel.LoadModel("Models/chessboard.obj");
	globeModel.LoadModel("Models/globe1.obj");
	wineCabinetModel.LoadModel("Models/wine.obj");
	paperModel.LoadModel("Models/paper.obj");
	drawerKeyModel.LoadModel("Models/drawerkey.obj");
	notebookModel.LoadModel("Models/notebook.obj");
	letterModel.LoadModel("Models/letter.obj");
	lesserSalomonModel.LoadModel("Models/lessersalomon.obj");
	lecternModel.LoadModel("Models/lectern.obj");
	carpetModel.LoadModel("Models/carpet.obj");
	//pageModel.LoadModel("Models/page.obj");



	doorsRoom1Model.LoadModel("Models/exitDoorsRoom1.obj");
	colliderWallModel.LoadModel("Models/invisibleWall.obj");

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
	sofaModel.LoadModel("Models/sofa.obj");
	candleModel.LoadModel("Models/candle.obj");
	lighterModel.LoadModel("Models/lighter.obj");
	batteryModel.LoadModel("Models/battery.obj");
	lowerChestModel.LoadModel("Models/lowerChest.obj");
	upperChestModel.LoadModel("Models/upperChest.obj");
	calenderModel.LoadModel("Models/calender.obj");
	tableModel.LoadModel("Models/table.obj");
	pentagramModel.LoadModel("Models/pentagram.obj");
	pentagram2Model.LoadModel("Models/pentagram2.obj");
	pedestalModel.LoadModel("Models/pedestal.obj");
	houseWallsModel.LoadModel("Models/projekt.obj");
	houseFloorModel.LoadModel("Models/podloga.obj");
	houseCeilingModel.LoadModel("Models/sufit.obj");
	candleHolderModel.LoadModel("Models/candleholder.obj");

	// Desk models
	deskModel.LoadModel("Models/desk.obj");
	deskDoorModel.LoadModel("Models/deskDoors.obj");
	deskDrawerModelTop.LoadModel("Models/drawerTop.obj");
	deskDrawerModelMiddle.LoadModel("Models/drawerMiddle.obj");
	deskDrawerModelBottom.LoadModel("Models/drawerBottom.obj");
	deskDrawerModels.push_back(&deskDrawerModelTop);
	deskDrawerModels.push_back(&deskDrawerModelMiddle);
	deskDrawerModels.push_back(&deskDrawerModelBottom);




	// Lock models
	lockBaseModel.LoadModel("Models/lockcz1.obj");
	lockRotatingModel1.LoadModel("Models/rol1.obj");
	lockRotatingModel2.LoadModel("Models/rol1.obj");
	lockRotatingModel3.LoadModel("Models/rol3.obj");
	lockRotatingModel4.LoadModel("Models/rol4.obj");
	lockRotatingModels.push_back(&lockRotatingModel1);
	lockRotatingModels.push_back(&lockRotatingModel2);
	lockRotatingModels.push_back(&lockRotatingModel3);
	lockRotatingModels.push_back(&lockRotatingModel4);
	lockMetalPartModel.LoadModel("Models/lockcz6.obj");



	flashlightEntity = new Entity(&flashlightModel, glm::vec3(5.0f, 2.0f, -3.0f), glm::vec3(0.0f), glm::vec3(0.03f));
	flashlightEntity->setCastsShadow(false);
	flashlightEntity->setTitle("Flashlight");

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
	tooltipRenderer = new TextRenderer(mainWindow.getBufferWidth(), mainWindow.getBufferHeight()); \
		smallerTooltipRenderer = new TextRenderer(mainWindow.getBufferWidth(), mainWindow.getBufferHeight());
	// Load font
	textRenderer->Load("Fonts/BitterPro-Medium.ttf", 12);
	tooltipRenderer->Load("Fonts/BitterPro-Bold.ttf", 36);
	smallerTooltipRenderer->Load("Fonts/BitterPro-Bold.ttf", 12);

	// Create scene
	scene = new Scene(camera, player, tooltipRenderer, smallerTooltipRenderer);

	/*sculpture.LoadModel("Models/rzezba.obj");*/


	// Create Entities
	framuga = new Entity(&framugaModel,glm::vec3(3.0f, -0.1f, -2.7f), glm::vec3(0.0f,-90.0f,0.0f), glm::vec3(1.41f));
	doorEntity = new Door(&door, glm::vec3(3.0f, -0.1f, -2.7f), glm::vec3(0.0f,-90.0f,0.0f), glm::vec3(1.4f), "Doors", framuga, "mainKey");
	doorEntity->setLocked(true);
	keyEntity = new Key(&keyModel, glm::vec3(-2.0f, 0.2f, 0.0f), glm::vec3(90.0f,0.0f,0.0f), glm::vec3(0.75f), "mainKey", keySprite, true);
	keyEntity->setTitle("Key");
	keyEntity->setColissions(false);
	radioEntity = new Radio(&radioModel, glm::vec3(-5.25f, 0.95f, -7.5f), glm::vec3(0.0f,30.0f,0.0f), glm::vec3(2.0f), true);
	radioEntity->setTitle("Radio");
	lighterEntity = new Lighter(&lighterModel, glm::vec3(1.4f, 0.0f,-0.2f), glm::vec3(0.0f,0.0f,90.0f), glm::vec3(3.0f), lighterSprite, true);
	lighterEntity->setTitle("Lighter");
	lighterEntity->setColissions(false);


	doorsRoom1Entity = new Door(&doorsRoom1Model, glm::vec3(-2.9f, -0.1f, 1.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(2.0f), "Doors", framuga, "finalExitKey");
	doorsRoom1Entity->setLocked(true);
	battery1Entity = new Battery(&batteryModel, glm::vec3(-2.5f, 0.5f, -3.0f), glm::vec3(0.0f), glm::vec3(2.5f),"battery", batterySprite, true);
	battery1Entity->setTitle("Battery");
	battery2Entity = new Battery(&batteryModel, glm::vec3(1.0f, 1.0f, -7.4f), glm::vec3(0.0f,0.0f,90.0f), glm::vec3(3.0f), "battery",batterySprite, true);
	battery2Entity->setTitle("Battery");
	calenderEntity = new Entity(&calenderModel, glm::vec3(-4.0f, 1.0f, -6.5f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(10.0f),true);
	calenderEntity->setTitle("Calender");
	calenderEntity->setExamineText("The date is December 26th.");
	tableEntity = new Entity(&tableModel, glm::vec3(1.0f, -0.2f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.011f), false);
	posterEntity = new Entity(&posterModel, glm::vec3(-6.0f, 1.5f, -5.49f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f), true);
	posterEntity->setTitle("Poster");
	posterEntity->setExamineText("A poster with morse code instruction. Maybe it will be useful");
	clockElementEntity = new ClockMovingPart(&clockElementModel, glm::vec3(2.5f, 2.0f, -5.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(3.5f), true);
	clockEntity = new Entity(&clockModel, glm::vec3(2.5f, 1.0f, -5.0f), glm::vec3(0.0f, 270.0f, 0.0f), glm::vec3(3.5f), true);
	clockEntity->setTitle("Clock");
	clockEntity->setExamineText("The clock is stuck at 8:15 PM. Thats weird...");
	globeEntity = new Entity(&globeModel, glm::vec3(-4.0f, 1.0f, -7.4f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f), true);
	globeEntity->setTitle("Globe");
	globeEntity->setExamineText("A globe showing the Earth.");
	chessboardEntity = new Entity(&chessboardModel, glm::vec3(1.0f, 0.6f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.3f), true);
	chessboardEntity->setTitle("Chessboard");
	chessboardEntity->setExamineText("An old chessboard.");
	paperEntity = new Readable(&paperModel, glm::vec3(-0.5f, -0.1f, 0.0f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(2.0f),spriteRenderer,paperSprite, true);
	paperEntity->setTitle("Missing Notebook's Page");
	letterEntity = new Readable(&paperModel, glm::vec3(-5.2f, 0.15f, -7.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.5f), spriteRenderer, letterSprite, true);
	letterEntity->setTitle("Mysterious Letter");
	lesserSalomonEntity = new Readable(&lesserSalomonModel, glm::vec3(1.5f, -1.3f, -9.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f), spriteRenderer, lesserSalomonSprite, true);
	lesserSalomonEntity->setTitle("Lesser Key of Solomon");
	lecternEntity = new Readable(&lecternModel, glm::vec3(17.0f, -0.1f, -0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.25f), spriteRenderer, lecternSprite, true);
	lecternEntity->setTitle("Lectern with ritual instructions");
	secondRoomDoorFramuga = new Entity(&framugaModel, glm::vec3(12.0f, -0.1f, -2.7f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.41f));
	secondRoomDoorEntity = new Door(&door, glm::vec3(12.0f, -0.1f, -2.7f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.4f),"Locked Doors", secondRoomDoorFramuga, "secondDoorsKey");
	secondRoomDoorEntity->setLocked(true);
	secondKeyEntity = new Key(&keyModel, glm::vec3(0.0f, 0.3f, -9.8f), glm::vec3(90.0f, 0.0f, 0.0f), glm::vec3(0.75f), "secondDoorsKey", keySprite, true);
	secondKeyEntity->setTitle("Key");
	//pageEntity = new Entity(&pageModel, glm::vec3(-5.0f, 1.0f, -3.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(2.0f), true);
	//pageEntity->setTitle("Page");
	//pageEntity->setColissions(false);

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


	// Room 1 collisions
	floorEntity = new Entity(&colliderWallModel, glm::vec3(-9.0f, 0.0f, 13.0f), glm::vec3(-90.0f,0.0f,0.0f), glm::vec3(40.0f, 24.0f, 10.0f));
	floorEntity->setCastsShadow(false);
	floorEntity->setVisibility(false);
	ceilingEntity = new Entity(&colliderWallModel, glm::vec3(-9.0f, 4.0f, 13.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(40.0f, 24.0f, 1.0f));
	ceilingEntity->setVisibility(false);
	room1BackWallLeftSideEntity = new Entity(&colliderWallModel, glm::vec3(-6.0f, -0.1f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.5f, 4.0f, 1.0f));
	ceilingEntity->setCastsShadow(false);
	room1BackWallRightSideEntity = new Entity(&colliderWallModel, glm::vec3(1.6f, -0.1f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	room1BackWallLeftSideEntity->setCastsShadow(false);
	room1BackWallLeftSideEntity->setVisibility(false);
	room1BackWallUpSideEntity = new Entity(&colliderWallModel, glm::vec3(0.5f, 1.6f, -8.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.2f, 3.0f, 1.0f));
	room1BackWallLeftSideEntity->setCastsShadow(false);
	room1LeftWallEntity = new Entity(&colliderWallModel, glm::vec3(-5.8f, -0.1f, 1.2f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(10.0f, 4.0f, 1.0f));
	room1LeftWallEntity->setCastsShadow(false);
	room1LeftWallEntity->setVisibility(false);
	room1RightWallRightEntity = new Entity(&colliderWallModel, glm::vec3(3.1f, -0.1f, 1.3f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(4.0f, 4.0f, 1.0f));
	room1RightWallRightEntity->setCastsShadow(false);
	room1RightWallRightEntity->setVisibility(false);
	room1RightWallLeftEntity = new Entity(&colliderWallModel, glm::vec3(3.1f, -0.1f, -3.9f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(8.0f, 4.0f, 1.0f));
	room1RightWallLeftEntity->setCastsShadow(false);
	room1RightWallLeftEntity->setVisibility(false);
	room1RightWallUpEntity = new Entity(&colliderWallModel, glm::vec3(3.1f, 2.8f, -2.5f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(2.0f, 1.5f, 1.0f));
	room1RightWallUpEntity->setCastsShadow(false);
	room1RightWallUpEntity->setVisibility(false);
	room1FrontWallUpEntity= new Entity(&colliderWallModel, glm::vec3(-6.0f, 3.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.5f, 1.0f, 1.0f));
	room1FrontWallUpEntity->setCastsShadow(false);
	room1FrontWallUpEntity->setVisibility(false);
	room1FrontWallLeftEntity = new Entity(&colliderWallModel, glm::vec3(-6.0f, -0.1f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 4.0f, 1.0f));
	room1FrontWallLeftEntity->setCastsShadow(false);
	room1FrontWallLeftEntity->setVisibility(false);
	room1FrontWallRightEntity = new Entity(&colliderWallModel, glm::vec3(-3.0f, -0.1f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f, 4.0f, 1.0f));
	room1FrontWallRightEntity->setCastsShadow(false);
	room1FrontWallRightEntity->setVisibility(false);

	houseWalls = new Entity(&houseWallsModel, glm::vec3(-6.0f, -0.1f, 1.0f), glm::vec3(180.0f, 0.0f, 0.0f), glm::vec3(1.0f), false);
	houseWalls->setColissions(false);
	houseFloor = new Entity(&houseFloorModel, glm::vec3(-6.0f, -0.1f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), false);
	houseFloor->setColissions(false);
	houseCeiling = new Entity(&houseCeilingModel, glm::vec3(2.0f, 4.9f, -18.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f), false);
	houseCeiling->setColissions(false);
	// Hidden room collisions
	hiddenRoomBackWallEntity = new Entity(&colliderWallModel, glm::vec3(-3.0f, -0.1f, -11.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(6.0f, 3.1f, 1.0f));
	hiddenRoomBackWallEntity->setCastsShadow(false);
	hiddenRoomLeftWallEntity = new Entity(&colliderWallModel, glm::vec3(-1.0f, -0.1f, -8.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(3.0f, 3.1f, 1.0f));
	hiddenRoomLeftWallEntity->setCastsShadow(false);
	hiddenRoomCeilingEntity = new Entity(&colliderWallModel, glm::vec3(-3.0f, 3.0f, -8.0f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(6.0f, 3.0f, 1.0f));
	hiddenRoomCeilingEntity->setCastsShadow(false);


	// Hidden room interior objects
	hiddenRoomChestEntity = new Chest(&lowerChestModel, &upperChestModel, glm::vec3(0.0f, 0.0f, -10.0f), glm::vec3(0.0f,180.0f,0.0f), glm::vec3(1.0f), scene, true);
	hiddenRoomLockEntity = new Lock(&lockBaseModel, glm::vec3(0.0f, 0.45f, -9.5f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(2.0f), lockRotatingModels, &lockMetalPartModel, scene, true);
	hiddenRoomLockEntity->setTitle("Lock");
	hiddenRoomLockEntity->setLockPassword(std::vector<int>{3, 5, 2, 4});
	hiddenRoomLockEntity->setChestToUnlock(hiddenRoomChestEntity);
	// Corridor collisions
	corridorLeftWallEntity = new Entity(&colliderWallModel, glm::vec3(3.0f, -0.1f, -6.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.8f, 4.0f, 1.0f));
	corridorLeftWallEntity->setCastsShadow(false);
	corridorLeftWallEntity->setVisibility(false);
	corridorRightWallEntity = new Entity(&colliderWallModel, glm::vec3(3.0f, -0.1f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(8.8f, 4.0f, 1.0f));
	corridorRightWallEntity->setCastsShadow(false);
	corridorRightWallEntity->setVisibility(false);

	// Second room collisions
	secondRoomLeftWallEntity = new Entity(&colliderWallModel, glm::vec3(12.0f, -0.3f, -9.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(10.0f, 4.5f, 1.0f));
	secondRoomLeftWallEntity->setVisibility(false);
	secondRoomLeftWallEntity->setCastsShadow(false);
	secondRoomRightWallEntity = new Entity(&colliderWallModel, glm::vec3(12.0f, -0.3f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(11.0f, 4.5f, 1.0f));
	secondRoomRightWallEntity->setVisibility(false);
	secondRoomRightWallEntity->setCastsShadow(false);
	secondRoomBackWallEntity = new Entity(&colliderWallModel, glm::vec3(22.0f, -0.3f, 2.5f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(12.0f, 4.5f, 1.0f));
	secondRoomBackWallEntity->setVisibility(false);
	secondRoomBackWallEntity->setCastsShadow(false);
	secondRoomFrontWallLeftEntity = new Entity(&colliderWallModel, glm::vec3(12.0f, -0.3f, -4.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(6.0f, 4.5f, 1.0f));
	secondRoomFrontWallLeftEntity->setVisibility(false);
	secondRoomFrontWallLeftEntity->setCastsShadow(false);
	secondRoomFrontWallRightEntity = new Entity(&colliderWallModel, glm::vec3(12.0f, -0.3f, 2.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(4.7f, 4.5f, 1.0f));
	secondRoomFrontWallRightEntity->setVisibility(false);
	secondRoomFrontWallRightEntity->setCastsShadow(false);

	// Corridor interior objects
	hauntedPaintingEntity1 = new HauntedEntity(&huntModel1, &huntModel2, glm::vec3(5.5f, 2.0f, -5.9f), glm::vec3(180.0f, 90.0f, 90.0f), glm::vec3(2.0f));
	hauntedPaintingEntity1->setTitle("The Royal Hunt (1840)");
	hauntedPaintingEntity1->setExamineText("An eerie painting depicting a royal hunt. There are exactly 4 hounds.");
	hauntedPaintingEntity2 = new HauntedEntity(&skullsModel1, &skullsModel2, glm::vec3(10.0f, 2.0f, -5.9f), glm::vec3(180.0f, 90.0f, 90.0f), glm::vec3(2.0f));
	hauntedPaintingEntity2->setTitle("Study of Mortality (1860)");
	hauntedPaintingEntity2->setExamineText("A disturbing painting showing library with human skulls. There are exactly 2 skulls.");
	hauntedPaintingEntity3 = new HauntedEntity(&ravensModel1, &ravensModel2, glm::vec3(5.5f, 2.0f, -1.1f), glm::vec3(180.0f, -90.0f, 90.0f), glm::vec3(2.0f));
	hauntedPaintingEntity3->setTitle("Harbingers of Doom (1880)");
	hauntedPaintingEntity3->setExamineText("A dark painting portraying a flock of ravens. There are exactly 5 ravens.");
	hauntedPaintingEntity4 = new HauntedEntity(&cultModel1, &cultModel2, glm::vec3(10.0f, 2.0f, -1.1f), glm::vec3(180.0f, -90.0f, 90.0f), glm::vec3(2.0f));
	hauntedPaintingEntity4->setTitle("The Gathering (1900)");
	hauntedPaintingEntity4->setExamineText("A mysterious painting illustrating a cult gathering. There are exactly 3 cultits.");

	pedestalLight1 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f, glm::vec3(5.5f, 1.5f, -5.0f), 0.5f, 0.7f, 1.8f, 10, 10.0f, 0.01f, 256.0f, 256.0f);
	pedestalLight2 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f, glm::vec3(10.0f, 1.5f, -5.0f), 0.5f, 0.7f, 1.8f, 11, 10.0f, 0.01f, 256.0f, 256.0f);
	pedestalLight3 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f, glm::vec3(5.5f, 1.5f, -2.0f), 0.5f, 0.7f, 1.8f, 12, 10.0f, 0.01f, 256.0f, 256.0f);
	pedestalLight4 = new PointLight(glm::vec3(1.0f, 0.0f, 0.0f), 0.0f, 1.0f, glm::vec3(10.0f, 1.5f, -2.0f), 0.5f, 0.7f, 1.8f, 13, 10.0f, 0.01f, 256.0f, 256.0f);
	outsideLight = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), 0.0f, 0.0f, glm::vec3(-4.0f, 3.0f, 2.0f), 2.0f, 0.7f, 1.8f, 14, 50.0f, 0.01f, 256.0f, 256.0f);





	pedestalEntity1 = new Pedestal(&pedestalModel, glm::vec3(5.5f, 0.0f, -5.0f), glm::vec3(0.0f, 90.0f, 0.0f), glm::vec3(0.5f),scene, pedestalLight1,true);
	pedestalEntity1->setTitle("Pedestal");
	pedestalEntity1->setCorrectItemTag("pedestal_collar");
	pedestalEntity2 = new Pedestal(&pedestalModel, glm::vec3(10.0f, 0.0f, -5.0f), glm::vec3(0.0f,90.0f,0.0f), glm::vec3(0.5f),scene, pedestalLight2, true);
	pedestalEntity2->setTitle("Pedestal");
	pedestalEntity2->setCorrectItemTag("pedestal_skull");
	pedestalEntity3 = new Pedestal(&pedestalModel, glm::vec3(5.5f, 0.0f, -2.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(0.5f),scene, pedestalLight3, true);
	pedestalEntity3->setTitle("Pedestal");
	pedestalEntity3->setCorrectItemTag("pedestal_feather");
	pedestalEntity4 = new Pedestal(&pedestalModel, glm::vec3(10.0f, 0.0f, -2.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(0.5f),scene, pedestalLight4, true);
	pedestalEntity4->setTitle("Pedestal");
	pedestalEntity4->setCorrectItemTag("pedestal_eye");
	std::vector<Pedestal*> pedestals = { pedestalEntity1, pedestalEntity2, pedestalEntity3, pedestalEntity4 };
	notebookEntity = new Entity(&notebookModel, glm::vec3(-4.5f, 1.0f, -7.4f), glm::vec3(-90.0f, 0.0f, 0.0f), glm::vec3(0.05f), true);
	notebookEntity->setTitle("Notebook");
	notebookEntity->setExamineText("A notebook containing strange drawings. Seems like some pages are missing.");

	witchesPaintingEntity = new Entity(&witchesModel, glm::vec3(22.0f, 1.9f, -3.5f), glm::vec3(180.0f, 0.0f, 90.0f), glm::vec3(2.4f), true);
	witchesPaintingEntity->setTitle("The Witches' Sabbath (1597)");
	witchesPaintingEntity->setColissions(false);
	/*sculptureEntity = new Entity(&sculpture, lessShinyMaterial, glm::vec3(-10.0f, -1.0f, -4.0f), glm::vec3(0.0f, 30.0f, 0.0f), glm::vec3(4.0f));
	sculptureEntity->setTitle("Sculpture");*/




	// Skybox
	skybox = new Skybox(skyboxFaces);


	mainLight = new DirectionalLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -5.0f, -5.5f), 0.0f, 0.0f, 256.0f, 256.0f);
	candleLight = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 0, 20.0f, 0.01f, 256.0f, 256.0f);
	candleLight2 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 1, 20.0f, 0.01f, 256.0f, 256.0f);
	candleLight3 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 2, 20.0f, 0.01f, 256.0f, 256.0f);
	candleLight4 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 3, 20.0f, 0.01f, 256.0f, 256.0f);
	candleLight5 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 4, 20.0f, 0.01f, 256.0f, 256.0f);
	candleLight6 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 5, 20.0f, 0.01f, 128.0f, 128.0f);
	candleLight6->setCastShadow(true);
	candleLight7 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 6, 20.0f, 0.01f, 128.0f, 128.0f);
	candleLight7->setCastShadow(true);
	candleLight8 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 7, 20.0f, 0.01f, 128.0f, 128.0f);
	candleLight8->setCastShadow(true);
	candleLight9 = new PointLight(glm::vec3(10.0f, 6.0f, 1.0f), 0.0f, 1.0f, glm::vec3(2.0f, 1.0f, -3.0f), 1.0f, 0.7f, 1.8f, 8, 20.0f, 0.01f, 128.0f,128.0f);
	candleLight9->setCastShadow(true);
	flashlight = new Flashlight(glm::vec3(4.0f, 3.8f, 2.8f), 0.5f, 1.0f, camera->getCameraPosition(), 1.0f, 0.045f, 0.0075f, camera->getCameraFront(), 16.5f, 25.0f, 1024.0f, 1024.0f);
	lampLight = new PointLight(glm::vec3(2.4f, 2.0f, 1.4f),0.05f, 2.0f,glm::vec3(-4.0f, 1.7f, -7.0f),1.0f, 0.15f, 0.032f,9, 20.0f, 0.01f, 1024.0f, 1024.0f);
	lampLight->setCastShadow(true);

	lampEntity = new Lamp(&lampModel, glm::vec3(2.3f, 0.0f, -1.5f), glm::vec3(0.0f), glm::vec3(0.2f), lampLight, true);
	lampEntity->setCastsShadow(false);
	lampEntity->setTitle("Lamp");



	deskEntity = new Desk(&deskModel, glm::vec3(-4.5f, -0.1f, -7.4f), glm::vec3(0.0f), glm::vec3(1.2f), deskDrawerModels, &deskDoorModel, scene , false);
	deskEntity->setDrawerContainedEntity(battery1Entity);
	drawerKeyEntity = new Key(&drawerKeyModel, glm::vec3(-5.5f, 0.7f, -3.25f), glm::vec3(0.0f), glm::vec3(2.0f), "drawerKey2", keySprite, true);
	drawerKeyEntity->setTitle("Key");
	chestEntity = new Chest(&lowerChestModel, &upperChestModel, glm::vec3(-2.0f, 0.0f, 0.15f), glm::vec3(0.0f), glm::vec3(1.0f), scene, true);
	chestEntity->setTitle("Chest");
	lockEntity = new Lock(&lockBaseModel, glm::vec3(-2.0f, 0.45f, -0.35f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f), lockRotatingModels, &lockMetalPartModel, scene, true);
	lockEntity->setLockPassword(std::vector<int>{1, 6, 1, 5});
	lockEntity->setTitle("Lock");
	lockEntity->setChestToUnlock(chestEntity);

	sofaEntity = new Entity(&sofaModel, glm::vec3(1.8f, 0.0f, -0.5f), glm::vec3(0.0f, -130.0f, 0.0f), glm::vec3(1.5f));
	candleEntity = new Candle(&candleModel, glm::vec3(15.7f, 0.0f, -3.3f), glm::vec3(0.0f), glm::vec3(4.0f),candleLight,true);
	candleEntity->setTitle("Candle");
	candleEntity->setTag("B");
	candle2Entity = new Candle(&candleModel, glm::vec3(17.6f, 0.0f, -5.5f), glm::vec3(0.0f), glm::vec3(4.0f), candleLight2, true);
	candle2Entity->setTitle("Candle");
	candle2Entity->setTag("A");
	candle3Entity = new Candle(&candleModel, glm::vec3(17.3f, 0.0f, -2.3f), glm::vec3(0.0f), glm::vec3(4.0f), candleLight3, true);
	candle3Entity->setTitle("Candle");
	candle3Entity->setTag("E");
	candle4Entity = new Candle(&candleModel, glm::vec3(15.7f, 0.0f, -5.1f), glm::vec3(0.0f), glm::vec3(4.0f), candleLight4, true);
	candle4Entity->setTitle("Candle");
	candle4Entity->setTag("L");
	candle5Entity = new Candle(&candleModel, glm::vec3(18.8f, 0.0f, -4.2f), glm::vec3(0.0f), glm::vec3(4.0f), candleLight5, true);
	candle5Entity->setTitle("Candle");
	wineCabinetEntity = new Entity(&wineCabinetModel, glm::vec3(-5.5f, 0.0f, -3.5f), glm::vec3(0.0f,270.0f, 0.0f), glm::vec3(1.0f), false);
	candles.push_back(candleEntity);
	candles.push_back(candle2Entity);
	candles.push_back(candle3Entity);
	candles.push_back(candle4Entity);
	candles.push_back(candle5Entity);
	carpetEntity = new Entity(&carpetModel, glm::vec3(6.0f, 0.0f, -2.5f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.175f), false);
	carpetEntity->setColissions(false);
	
	candle6Entity = new Candle(&candleHolderModel, glm::vec3(16.5f, 1.3f, -8.8f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.25f), candleLight6, false);
	candle7Entity = new Candle(&candleHolderModel, glm::vec3(21.8f, 1.3f, -5.0f), glm::vec3(0.0f,-90.0f,0.0f), glm::vec3(0.25f), candleLight7, false);
	candle8Entity = new Candle(&candleHolderModel, glm::vec3(21.8f, 1.3f, -1.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(0.25f), candleLight8, false);
	candle9Entity = new Candle(&candleHolderModel, glm::vec3(16.5f, 1.3f, 1.9f), glm::vec3(0.0f, 180.0f, 0.0f), glm::vec3(0.25f), candleLight9, false);
	candles.push_back(candle6Entity);
	candles.push_back(candle7Entity);
	candles.push_back(candle8Entity);
	candles.push_back(candle9Entity);



	collarEntity = new Pickable(&collarModel, glm::vec3(16.5f, 0.0f, -7.7f), glm::vec3(0.0f), glm::vec3(0.05f),"pedestal_collar", collarSprite, true);
	collarEntity->setTitle("Collar");
	featherEntity = new Pickable(&featherModel, glm::vec3(20.0f, 0.0f, -2.0f), glm::vec3(0.0f), glm::vec3(0.5f), "pedestal_feather", crowSprite, true);
	featherEntity->setTitle("Crow");
	eyeEntity = new Pickable(&eyeModel, glm::vec3(12.5f, 0.0f, 1.2f), glm::vec3(0.0f), glm::vec3(0.125f), "pedestal_eye", eyeSprite, true);
	eyeEntity->setTitle("Human Eye");
	skullEntity = new Pickable(&skullModel, glm::vec3(13.0f, 0.0f, -6.5f), glm::vec3(0.0f), glm::vec3(1.5f), "pedestal_skull", skullSprite, true);
	skullEntity->setTitle("Skull");

	std::vector<Candle*> pentagramCandles = { candleEntity, candle2Entity, candle3Entity, candle4Entity, candle5Entity };
	std::vector<Candle*> wallCandles = { candle6Entity, candle7Entity, candle8Entity, candle9Entity };	
	candlePuzzle = new CandlePuzzle(pentagramCandles,wallCandles, pedestals, doorEntity);
	pedestalPuzzle = new PedestalPuzzle(pedestals, doorEntity, doorsRoom1Entity);


	pentagramEntity = new HauntedEntity(&pentagramModel,&pentagram2Model, glm::vec3(17.2f, -0.2f, -4.0f), glm::vec3(0.0f, -40.0f, 0.0f), glm::vec3(1.5f), false);
	pentagramEntity->setTitle("Pentagram");
	pentagramEntity->setColissions(false);
	// Add entities and lights to scene
	scene->AddPointLight(candleLight);
	scene->AddPointLight(candleLight2);
	scene->AddPointLight(candleLight3);
	scene->AddPointLight(candleLight4);
	scene->AddPointLight(candleLight5);
	scene->AddPointLight(candleLight6);
	scene->AddPointLight(candleLight7);
	scene->AddPointLight(candleLight8);
	scene->AddPointLight(candleLight9);
	scene->AddPointLight(lampLight);
	scene->AddPointLight(pedestalLight1);
	scene->AddPointLight(pedestalLight2);
	scene->AddPointLight(pedestalLight3);
	scene->AddPointLight(pedestalLight4);
	scene->AddPointLight(outsideLight);
	scene->SetDirectionalLight(mainLight);
	scene->SetFlashlight(flashlight);
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
	scene->AddEntity(candleEntity);
	scene->AddEntity(candle2Entity);
	scene->AddEntity(candle3Entity);
	scene->AddEntity(candle4Entity);
	scene->AddEntity(candle5Entity);
	scene->AddEntity(battery1Entity);
	scene->AddEntity(battery2Entity);
	scene->AddEntity(chestEntity);
	scene->AddEntity(calenderEntity);
	scene->AddEntity(tableEntity);
	scene->AddEntity(lampEntity);
	scene->AddEntity(globeEntity);
	scene->AddEntity(chessboardEntity);

	scene->AddEntity(paperEntity);
	scene->AddEntity(letterEntity);
	scene->AddEntity(lesserSalomonEntity);
	scene->AddEntity(lecternEntity);
	scene->AddEntity(secondRoomDoorEntity);
	scene->AddEntity(secondRoomDoorFramuga);
	scene->AddEntity(secondKeyEntity);
	//scene->AddEntity(carpetEntity);


	scene->AddEntity(doorEntity);
	scene->AddEntity(flashlightEntity);
	scene->AddEntity(framuga);
	scene->AddEntity(keyEntity);
	scene->AddEntity(radioEntity);
	//scene->AddEntity(pageEntity);
	scene->AddEntity(lockEntity);
	scene->AddEntity(deskEntity);
	scene->AddEntity(sofaEntity);
	scene->AddEntity(lighterEntity);
	scene->AddEntity(posterEntity);
	scene->AddEntity(clockElementEntity);
	scene->AddEntity(clockEntity);
	scene->AddEntity(notebookEntity);

	// Room 1 collisions
	scene->AddEntity(floorEntity);
	scene->AddEntity(ceilingEntity);
	scene->AddEntity(room1BackWallLeftSideEntity);
	scene->AddEntity(room1BackWallRightSideEntity);
	scene->AddEntity(room1BackWallUpSideEntity);
	scene->AddEntity(room1LeftWallEntity);
	scene->AddEntity(room1RightWallRightEntity);
	scene->AddEntity(room1RightWallLeftEntity);
	scene->AddEntity(room1RightWallUpEntity);
	scene->AddEntity(room1FrontWallUpEntity);
	scene->AddEntity(room1FrontWallLeftEntity);
	scene->AddEntity(room1FrontWallRightEntity);


	// Hidden room collisions
	scene->AddEntity(hiddenRoomBackWallEntity);
	scene->AddEntity(hiddenRoomLeftWallEntity);
	scene->AddEntity(hiddenRoomCeilingEntity);

	// Corridor collisions
	scene->AddEntity(corridorLeftWallEntity);
	scene->AddEntity(corridorRightWallEntity);

	// Second room collisions
	scene->AddEntity(secondRoomLeftWallEntity);
	scene->AddEntity(secondRoomRightWallEntity);
	scene->AddEntity(secondRoomBackWallEntity);
	scene->AddEntity(secondRoomFrontWallLeftEntity);
	scene->AddEntity(secondRoomFrontWallRightEntity);



	// Corridor interior objects
	scene->AddEntity(hauntedPaintingEntity1);
	scene->AddEntity(hauntedPaintingEntity2);
	scene->AddEntity(hauntedPaintingEntity3);
	scene->AddEntity(hauntedPaintingEntity4);
	scene->AddEntity(witchesPaintingEntity);
	scene->AddEntity(pedestalEntity1);
	scene->AddEntity(pedestalEntity2);
	scene->AddEntity(pedestalEntity3);
	scene->AddEntity(pedestalEntity4);

	// Hidden room interior objects
	scene->AddEntity(hiddenRoomChestEntity);
	scene->AddEntity(hiddenRoomLockEntity);

	/*scene->AddEntity(sculptureEntity);*/

	scene->AddEntity(pentagramEntity);
	scene->AddEntity(collarEntity);
	scene->AddEntity(featherEntity);
	scene->AddEntity(eyeEntity);
	scene->AddEntity(skullEntity);
	scene->AddEntity(houseWalls);
	scene->AddEntity(houseFloor);
	scene->AddEntity(houseCeiling);
	scene->AddEntity(wineCabinetEntity);
	scene->AddEntity(drawerKeyEntity);
	scene->AddEntity(candle6Entity);
	scene->AddEntity(candle7Entity);
	scene->AddEntity(candle8Entity);	
	scene->AddEntity(candle9Entity);
	return scene;
}

void DirectionalLightShadowMapPass() {
	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->UseShader();
	glViewport(0, 0, mainLight->getShadowMap()->getShadowWidth(), mainLight->getShadowMap()->getShadowHeight());
	glCullFace(GL_BACK);
	mainLight->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lightTransform = mainLight->CalculateLightTransform();
	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->setMat4("lightSpaceTransform", lightTransform);

	scene->RenderShadowMap(shaderList[SHADER_DIRLIGHT_SHADOWMAP]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void FlashlightShadowMapPass() {

	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->UseShader();
	glViewport(0, 0, flashlight->getShadowMap()->getShadowWidth(), flashlight->getShadowMap()->getShadowHeight());
	glCullFace(GL_BACK);
	flashlight->getShadowMap()->Write();
	glClear(GL_DEPTH_BUFFER_BIT);

	glm::mat4 lightTransform = flashlight->CalculateLightTransform();

	shaderList[SHADER_DIRLIGHT_SHADOWMAP]->setMat4("lightSpaceTransform", lightTransform);

	scene->RenderShadowMap(shaderList[SHADER_DIRLIGHT_SHADOWMAP]);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmniShadowMapPass(PointLight* pLight) {

	// Use the omni-directional shadow map shader
	shaderList[SHADER_OMNI_SHADOWMAP]->UseShader();
	// Setup viewport
	glViewport(0, 0, pLight->getShadowMap()->getShadowWidth(), pLight->getShadowMap()->getShadowHeight());
	glCullFace(GL_BACK);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderScenePass(glm::mat4 projectionMatrix)
{
	hdrBuffer->Bind();
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


	// Render particles
	shaderList[SHADER_PARTICLE]->UseShader();
	shaderList[SHADER_PARTICLE]->setMat4("projection", projectionMatrix);
	shaderList[SHADER_PARTICLE]->setMat4("view", camera.getViewMatrix());
	fireParticleSystem->Draw();

	// Render flashlight last (to be in front of all objects)
	glDepthMask(GL_TRUE);
	glStencilMask(0xFF);
	glCullFace(GL_BACK);
	glDisable(GL_STENCIL_TEST);



	if (player->getHeldEntity()) {
		if (gameState == STATE_MINIGAME) {
			return;
		}
		scene->RenderHeldEntity(shaderList[SHADER_DEFAULT], projectionMatrix);
	}


	hdrBuffer->Unbind();
}

void HandleKeyboardInput(float deltaTime, Scene* currentScene) {

	if (gameState == STATE_MINIGAME) {
		if(mainWindow.getKeys()[GLFW_KEY_ESCAPE]) {
			mainWindow.getKeys()[GLFW_KEY_ESCAPE] = false;
			camera.setPosition(savedCameraPosition);
			camera.setPitch(savedCameraPitch);
			camera.setYaw(savedCameraYaw);
			camera.updateCameraVectors();
			currentActiveLock->getLockRolls()[currentActiveLock->getSelectedIndex()]->setSelected(false);
			currentActiveLock = nullptr;
			SetGameState(STATE_PLAYING);
		}
		if(mainWindow.getKeys()[GLFW_KEY_E]) {
			mainWindow.getKeys()[GLFW_KEY_E] = false;
			// right
			currentActiveLock->changeSelectedIndex(1);
		}
		if(mainWindow.getKeys()[GLFW_KEY_Q]) {
			mainWindow.getKeys()[GLFW_KEY_Q] = false;
			// left
			currentActiveLock->changeSelectedIndex(-1);
		}
		if (mainWindow.getKeys()[GLFW_KEY_W]) {
			mainWindow.getKeys()[GLFW_KEY_W] = false;
			currentActiveLock->moveLockRolls(1);
			// gora
		}
		if (mainWindow.getKeys()[GLFW_KEY_S]) {
			mainWindow.getKeys()[GLFW_KEY_S] = false;
			currentActiveLock->moveLockRolls(-1);
			// dol
		}
	}

	if(gameState == STATE_READING) {
		if(mainWindow.getKeys()[GLFW_KEY_E] || mainWindow.getKeys()[GLFW_KEY_ESCAPE]) {
			mainWindow.getKeys()[GLFW_KEY_E] = false;
			mainWindow.getKeys()[GLFW_KEY_ESCAPE] = false;
			currentReadableEntity->setReadingState(false);
			currentReadableEntity = nullptr;
			SetGameState(STATE_PLAYING);
		}
		return;
	}

	if (mainWindow.getKeys()[GLFW_KEY_E])
	{
		mainWindow.getKeys()[GLFW_KEY_E] = false;

		Entity* target = player->getTargettedEntity();
		if (!target) return;
		if (!target->getInteractable()) return;
		target->Interact(player->getInventory());
		Lock* lockTarget = dynamic_cast<Lock*>(target);
		if (lockTarget != nullptr) {
			lockTarget->setOutlined(false);
			lockTarget->getLockRolls()[0]->setSelected(true);
			savedCameraPosition = camera.getCameraPosition();
			savedCameraPitch = camera.getPitch();
			savedCameraYaw = camera.getYaw();
			currentActiveLock = lockTarget;
			camera.setPosition(lockTarget->getLockPuzzlePos());
			camera.setPitch(lockTarget->getLockPuzzlePitch());
			camera.setYaw(lockTarget->getLockPuzzleYaw());
			camera.updateCameraVectors();
			
			SetGameState(STATE_MINIGAME);
		}
		Readable* readableTarget = dynamic_cast<Readable*>(target);
		if (readableTarget != nullptr) {
			readableTarget->setOutlined(false);
			currentReadableEntity = readableTarget;
			SetGameState(STATE_READING);
		}
		if (!target->getPickable()) return;
		player->pickUpEntity(target);
		return;
	}

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
	//if (mainWindow.getKeys()[GLFW_KEY_7]) {
	//	player->getInventory()->SetCurrentItem(6);
	//	mainWindow.getKeys()[GLFW_KEY_7] = false;
	//}
	//if (mainWindow.getKeys()[GLFW_KEY_8]) {
	//	player->getInventory()->SetCurrentItem(7);
	//	mainWindow.getKeys()[GLFW_KEY_8] = false;
	//}
	//if (mainWindow.getKeys()[GLFW_KEY_9]) {
	//	player->getInventory()->SetCurrentItem(8);
	//	mainWindow.getKeys()[GLFW_KEY_9] = false;
	//}


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


	if (gameState == STATE_READING) {
		Texture* pageTexture = currentReadableEntity->getPageTexture();
		spriteRenderer->DrawSprite(pageTexture, glm::vec2((uiWidth - pageTexture->getWidth()) / 2.0f, (uiHeight - pageTexture->getHeight()) / 2.0f), glm::vec2(pageTexture->getWidth(),pageTexture->getHeight()));
	}

	
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

	if (gameState == STATE_READING) {

		std::string message = "[E] / [ESC] Close";
		float scale = 1.0f;
		float textWidth = subtitlesRenderer->GetTextWidth(message) * scale;
		float xPos = (uiWidth - textWidth) / 2.0f;
		float yPos = uiHeight - 100.0f;
		subtitlesRenderer->RenderText(message, xPos, yPos, scale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	if (gameState == STATE_MINIGAME) {
		std::string message = "[W]/[S] Rotate   [Q]/[S] Select   [ESC] Exit";
		float scale = 1.0f;
		float textWidth = subtitlesRenderer->GetTextWidth(message) * scale;
		float xPos = (uiWidth - textWidth) / 2.0f;
		float yPos = uiHeight - 100.0f;
		subtitlesRenderer->RenderText(message, xPos, yPos, scale, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	}
	player->getInventory()->DrawNotification(deltaTime);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
}

void DrawMainMenu() {
	//gameUI->DrawMainMenu();
}


void PostProcessingPass() {
	bool horizontal = true;
	bool first_iteration = true;
	int amount = 5;

	shaderList[SHADER_BLOOM]->UseShader();

	for (unsigned int i = 0; i < amount; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, hdrBuffer->getPingPongFBO(horizontal));

		shaderList[SHADER_BLOOM]->setInt("horizontal", horizontal);

		glActiveTexture(GL_TEXTURE0);

		if (first_iteration)
		{
			glBindTexture(GL_TEXTURE_2D, hdrBuffer->getBrightTexture());
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, hdrBuffer->getPingPongColorBuffer(!horizontal));
		}

		shaderList[SHADER_BLOOM]->setInt("image", 0);

		hdrBuffer->RenderQuad();
		horizontal = !horizontal;
		first_iteration = false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shaderList[SHADER_POST_PROCESSING]->UseShader();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, hdrBuffer->getColorBuffer());
	shaderList[SHADER_POST_PROCESSING]->setInt("hdrBuffer", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, hdrBuffer->getPingPongColorBuffer(!horizontal)); 
	shaderList[SHADER_POST_PROCESSING]->setInt("bloomBlur", 1);
	shaderList[SHADER_POST_PROCESSING]->setFloat("exposure", exposure);
	hdrBuffer->RenderQuad();
}