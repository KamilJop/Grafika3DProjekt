#include "Scene.h"

Scene::Scene(Camera* cam, Player* play)
{
	camera = cam;
	player = play;
	dirLight = nullptr;
	flashLight = nullptr;
}
Scene::~Scene()
{
	entities.clear();
	pointLights.clear();
	dirLight = nullptr;
	flashLight = nullptr;
	camera = nullptr;
}

void Scene::Render(Shader* shader, glm::mat4 projection)
{
	shader->UseShader();
	shader->setMat4("projection", projection);
	shader->setMat4("view", camera->getViewMatrix());
	shader->setVec3("cameraPosition", camera->getCameraPosition());
	shader->setMat4("directionalLightSpaceTransform", dirLight->CalculateLightTransform());
	shader->setMat4("flashLightSpaceTransform", flashLight->CalculateLightTransform());
	for (auto& pLight : pointLights)
	{
		pLight->useLight(shader);
	}
	if (dirLight)
	{
		dirLight->useLight(shader);
	}
	glm::vec3 camPos = camera->getCameraPosition();
	glm::vec3 camFront = camera->getCameraFront();
	glm::vec3 camRight = camera->getCameraRight();
	glm::vec3 camUp = camera->getCameraUp();

	glm::vec3 offset;
	offset.x = +0.25f;
	offset.y = -0.20f;
	offset.z = +0.35f;

	glm::vec3 finalPos =
		camPos +
		camRight * offset.x +
		camUp * offset.y +
		camFront * offset.z;
	player->updateFlashlightPosition(finalPos);
	if (flashLight && player->getFlashlightState())
	{
		glm::vec3 lightSourcePos = finalPos + (camFront * 0.3f);
		flashLight->setLightPosition(lightSourcePos);
		flashLight->setLightDirection(camFront);

		flashLight->useLight(shader);
	}
	else
	{
		shader->setFloat("flashLight.ambientIntensity", 0.0f);
		shader->setFloat("flashLight.diffuseIntensity", 0.0f);
	}

	for (auto& entity : entities)
	{
		entity->DrawEntity(shader);
	}
	
	
	
}

void Scene::Update(float deltaTime)
{
	for (auto& entity : entities)
	{
		entity->Update(deltaTime);
	}
}	


void Scene::RenderShadowMap(Shader* shadowShader)
{
	for (auto& entity : entities)
	{
		if (!entity->getCastsShadow())
			continue;
		shadowShader->setMat4("model", entity->GetModelMatrix());
		entity->DrawEntity(shadowShader);
	}
}