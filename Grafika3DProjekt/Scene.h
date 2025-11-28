#pragma once
#include <vector>
#include "Entity.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Flashlight.h"
#include "Camera.h"
#include "Player.h"
class Scene
{
public:
	std::vector<Entity*> entities;
	std::vector<PointLight*> pointLights;
	DirectionalLight* dirLight;
	Flashlight* flashLight;
	Camera* camera;
	Player* player;

	Scene(Camera* cam, Player* play);
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
	void Update(float deltaTime);
	void RenderShadowMap(Shader* shadowShader);


	

};

