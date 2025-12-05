#pragma once
#include <vector>
#include "Entities/Entity.h"
#include "Rendering/Shader.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/Flashlight.h"
#include "Camera.h"
#include "Player.h"
#include "Systems/TextRenderer.h"
#include "Config.h"
class Scene
{
public:
	std::vector<Entity*> entities;
	std::vector<PointLight*> pointLights;
	DirectionalLight* dirLight;
	Flashlight* flashLight;
	Camera* camera;
	Player* player;
	TextRenderer* textRenderer;
	Config& config = Config::getInstance();

	Scene(Camera* cam, Player* play, TextRenderer* renderer);
	~Scene();
	void AddEntity(Entity* entity)
	{
		entities.push_back(entity);
	}
	void AddPointLight(PointLight* light)
	{
		pointLights.push_back(light);
	}
	void SetDirectionalLight(DirectionalLight* light)
	{
		dirLight = light;
	}
	void SetFlashlight(Flashlight* light)
	{
		flashLight = light;
	}
	Camera *GetCamera()
	{
		return camera;
	}
	void RenderWithOutline (Shader* shader, glm::mat4 projection);
	void RenderWithoutOutline(Shader* shader, glm::mat4 projection);
	void RenderFlashlightEntity(Shader* shader, glm::mat4 projection);
	void Update(float deltaTime);
	void RenderShadowMap(Shader* shadowShader);
	void RenderTooltip(Entity* selectedEntity);
	std::vector<Entity*> getEntities() { return entities; }



private:
	void RenderLogic(Shader* shader, glm::mat4 projection);
	Entity* getEntityByTitle(const std::string& title);
	

};

