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

void Scene::RenderWithoutOutline(Shader* shader, glm::mat4 projection)
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

	if (flashLight && player->getFlashlightState())
	{
		flashLight->useLight(shader);
	}
	else
	{
		shader->setFloat("flashLight.ambientIntensity", 0.0f);
		shader->setFloat("flashLight.diffuseIntensity", 0.0f);
	}

	for (auto& entity : entities)
	{
		if (!entity->isOutlined()) {
			entity->DrawEntity(shader);
		}
	}
		
}

void Scene::RenderWithOutline(Shader* shader, glm::mat4 projection)
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

	if (flashLight && player->getFlashlightState())
	{
		flashLight->useLight(shader);
	}
	else
	{
		shader->setFloat("flashLight.ambientIntensity", 0.0f);
		shader->setFloat("flashLight.diffuseIntensity", 0.0f);
	}

	for (auto& entity : entities)
	{
		if (entity->isOutlined()) {
			entity->DrawEntity(shader);
		}
	}

}

void Scene::Update(float deltaTime)
{
	// Update player physics
	player->UpdatePhysics(deltaTime, entities);

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

	if (flashLight && player->getFlashlightState())
	{
		if (player->walkTimer > 0.0f)
		{
			float bobFrequency = 10.0f;
			float bobAmountX = 0.025f;
			float bobAmountY = 0.015f;
			float bobX = cos(player->walkTimer * bobFrequency / 2.0f) * bobAmountX;
			float bobY = sin(player->walkTimer * bobFrequency) * bobAmountY;
			finalPos += camera->Right * bobX;
			finalPos += camera->Up * bobY;
		}

		glm::vec3 lightSourcePos = finalPos + (camFront * 0.3f);
		flashLight->setLightPosition(lightSourcePos);
		flashLight->setLightDirection(camFront);
	}
	player->updateFlashlightPosition(finalPos);


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