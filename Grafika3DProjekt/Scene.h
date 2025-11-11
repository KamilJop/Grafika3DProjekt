#pragma once
#include <vector>
#include "Entity.h"
#include "Shader.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "Flashlight.h"
#include "Camera.h"
class Scene
{
public:
	std::vector<Entity*> entities;
	std::vector<PointLight*> pointLights;
	DirectionalLight* dirLight;
	Flashlight* flashLight;
	Camera* camera;

	Scene(Camera* cam);
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
	void Render(Shader* shader, glm::mat4 projection);
	void Update(float deltaTime);


	

};

