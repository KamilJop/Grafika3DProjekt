#include "Scene.h"

Scene::Scene(Camera* cam, Player* play, TextRenderer* renderer)
{
	camera = cam;
	player = play;
	dirLight = nullptr;
	flashLight = nullptr;
	textRenderer = renderer;
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
	RenderLogic(shader, projection);

	for (auto& entity : entities)
	{
		if (!entity->isOutlined()) {
			if (entity->getTitle() == "Flashlight") continue;
			entity->DrawEntity(shader);
		}
	}
		
}

void Scene::RenderWithOutline(Shader* shader, glm::mat4 projection)
{
	RenderLogic(shader, projection);

	for (auto& entity : entities)
	{
		if (entity->isOutlined()) {
			if (entity->getTitle() == "Flashlight") continue;
			entity->DrawEntity(shader);
			RenderTooltip(entity);
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

	player->checkTargettedEntity(entities);
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

void Scene::RenderLogic(Shader* shader, glm::mat4 projection)
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
}

void Scene::RenderTooltip(Entity* selectedEntity)
{
	float offset = textRenderer->GetTextWidth(selectedEntity->title) / 2.0f;

	textRenderer->RenderText( selectedEntity->title, 640.0f - offset, 670.0f, 1.0f, glm::vec3(1.0f, 0.0f, 1.0f));
}


Entity* Scene::getEntityByTitle(const std::string& title)
{
	for (auto& entity : entities)
	{
		if (entity->getTitle() == title)
			return entity;
	}
	return nullptr;
}

void Scene::RenderFlashlightEntity(Shader* shader, glm::mat4 projection)
{
	RenderLogic(shader, projection);
	Entity* flashlightEntity = getEntityByTitle("Flashlight");
	if (flashlightEntity)
	{
		flashlightEntity->DrawEntity(shader);
	}
}