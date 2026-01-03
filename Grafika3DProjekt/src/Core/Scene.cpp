#include "Scene.h"

/**
 * @brief Constructs a new Scene object.
 *
 * Initializes camera, player reference, text renderers, and sets lights to null.
 *
 * @param cam Pointer to the main camera.
 * @param play Pointer to the player object.
 * @param renderer Primary text renderer (large text).
 * @param secondRenderer Secondary text renderer (smaller text).
 */
Scene::Scene(Camera* cam, Player* play, TextRenderer* renderer, TextRenderer* secondRenderer)
{
    camera = cam;
    player = play;
    dirLight = nullptr;
    flashLight = nullptr;
    textRenderer = renderer;
    smallerTextRenderer = secondRenderer;
}

/**
 * @brief Scene destructor.
 *
 * Clears entity and light lists and resets pointers.
 */
Scene::~Scene()
{
    entities.clear();
    pointLights.clear();
    dirLight = nullptr;
    flashLight = nullptr;
    camera = nullptr;
}

/**
 * @brief Renders all non‑outlined entities.
 *
 * Applies shader logic, skips invisible or flashlight entities,
 * and handles selected entity highlighting.
 *
 * @param shader Shader used for rendering.
 * @param projection Projection matrix.
 */
void Scene::RenderWithoutOutline(Shader* shader, glm::mat4 projection)
{
    RenderLogic(shader, projection);

    for (auto& entity : entities)
    {
        if (!entity->isOutlined())
        {
            if (!entity->getVisible()) continue;
            if (entity->getTitle() == "Flashlight") continue;

            shader->setInt("isSelected", entity->isSelected() ? 1 : 0);
            entity->DrawEntity(shader);
        }
    }
}

/**
 * @brief Renders outlined entities and displays tooltips above them.
 *
 * @param shader Shader used for rendering.
 * @param projection Projection matrix.
 * @param w Screen width.
 * @param h Screen height.
 */
void Scene::RenderWithOutline(Shader* shader, glm::mat4 projection, int w, int h)
{
    RenderLogic(shader, projection);

    for (auto& entity : entities)
    {
        if (entity->isOutlined())
        {
            if (entity->getTitle() == "Flashlight") continue;

            entity->DrawEntity(shader);
            RenderTooltip(entity, w, h);
        }
    }
}

/**
 * @brief Updates the scene each frame.
 *
 * Handles:
 * - Player physics
 * - Flashlight bobbing and light updates
 * - Held entity positioning
 * - Entity updates and destruction
 * - Targeted entity detection
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Scene::Update(float deltaTime)
{
    // Update player physics
    player->UpdatePhysics(deltaTime, entities);

    // Camera vectors
    glm::vec3 camPos = camera->getCameraPosition();
    glm::vec3 camFront = camera->getCameraFront();
    glm::vec3 camRight = camera->getCameraRight();
    glm::vec3 camUp = camera->getCameraUp();

    // Offset for held item
    glm::vec3 offset(0.25f, -0.20f, 0.35f);

    glm::vec3 finalPos =
        camPos +
        camRight * offset.x +
        camUp * offset.y +
        camFront * offset.z;

    // Flashlight auto‑off if not selected
    if (player->getInventory()->GetCurrentItem()->tag != "flashlight")
    {
        player->changeFlashlightState(false);
    }

    // Flashlight bobbing + light update
    if (flashLight && player->getFlashlightState())
    {
        if (player->getWalkTimer() > 0.0f)
        {
            float bobFrequency = 10.0f;
            float bobAmountX = 0.025f;
            float bobAmountY = 0.015f;

            float bobX = cos(player->getWalkTimer() * bobFrequency / 2.0f) * bobAmountX;
            float bobY = sin(player->getWalkTimer() * bobFrequency) * bobAmountY;

            finalPos += camera->getCameraRight() * bobX;
            finalPos += camera->getCameraUp() * bobY;
        }

        glm::vec3 lightSourcePos = finalPos + (camFront * 0.05f);
        flashLight->setLightPosition(lightSourcePos);
        flashLight->setLightDirection(camFront);
    }

    // Update held entity (weapon/item)
    player->updateHeldEntityPosition(finalPos);

    // Update all entities and remove destroyed ones
    auto it = entities.begin();
    while (it != entities.end())
    {
        (*it)->Update(deltaTime);

        if ((*it)->getShouldGetDestroyed())
        {
            delete *it;
            it = entities.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Update targeted entity
    player->checkTargettedEntity(entities);
}

/**
 * @brief Renders all shadow‑casting entities into the shadow map.
 *
 * @param shadowShader Shader used for shadow rendering.
 */
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

/**
 * @brief Applies common shader uniforms for scene rendering.
 *
 * Sets:
 * - Projection & view matrices
 * - Camera position
 * - Light transforms
 * - Point lights
 * - Directional and flashlight states
 *
 * @param shader Shader to configure.
 * @param projection Projection matrix.
 */
void Scene::RenderLogic(Shader* shader, glm::mat4 projection)
{
    shader->UseShader();
    shader->setMat4("projection", projection);
    shader->setMat4("view", camera->getViewMatrix());
    shader->setVec3("cameraPosition", camera->getCameraPosition());

    shader->setMat4("directionalLightSpaceTransform", dirLight->CalculateLightTransform());
    shader->setMat4("flashLightSpaceTransform", flashLight->CalculateLightTransform());

    shader->setInt("isSelected", 0);

    for (auto& pLight : pointLights)
        pLight->useLight(shader);

    if (dirLight)
        dirLight->useLight(shader);

    if (flashLight && player->getFlashlightState())
        flashLight->useLight(shader);
    else
    {
        shader->setFloat("flashLight.ambientIntensity", 0.0f);
        shader->setFloat("flashLight.diffuseIntensity", 0.0f);
    }
}

/**
 * @brief Renders tooltip text above an outlined entity.
 *
 * Displays:
 * - Entity title
 * - Interaction hint (e.g., "[ E ]")
 *
 * @param selectedEntity Entity being hovered.
 * @param w Screen width.
 * @param h Screen height.
 */
void Scene::RenderTooltip(Entity* selectedEntity, int w, int h)
{
    std::string text = selectedEntity->getTitle();
    float offset = textRenderer->GetTextWidth(text) / 2.0f;

    textRenderer->RenderText(
        text,
        w / 2 - offset,
        h - 50.0f,
        1.0f,
        glm::vec4(config.highlightColor[0], config.highlightColor[1], config.highlightColor[2], 1.0f)
    );

    std::string tooltipText = "[ " + selectedEntity->GetActionText() + " ]";
    float tooltipOffset = smallerTextRenderer->GetTextWidth(tooltipText) / 2.0f;

    smallerTextRenderer->RenderText(
        tooltipText,
        w / 2 - tooltipOffset,
        h - 80.0f,
        1.0f,
        glm::vec4(config.highlightColor[0], config.highlightColor[1], config.highlightColor[2], 1.0f)
    );
}

/**
 * @brief Finds an entity by its title.
 *
 * @param title Title to search for.
 * @return Pointer to entity or nullptr if not found.
 */
Entity* Scene::getEntityByTitle(const std::string& title)
{
    for (auto& entity : entities)
    {
        if (entity->getTitle() == title)
            return entity;
    }
    return nullptr;
}

/**
 * @brief Renders the flashlight entity in first‑person view.
 *
 * @param shader Shader used for rendering.
 * @param projection Projection matrix.
 */
void Scene::RenderFlashlightEntity(Shader* shader, glm::mat4 projection)
{
    RenderLogic(shader, projection);

    Entity* flashlightEntity = getEntityByTitle("Flashlight");
    if (flashlightEntity)
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        flashlightEntity->DrawEntity(shader);
    }
}

/**
 * @brief Renders the held entity (weapon/item) in first‑person view.
 *
 * @param shader Shader used for rendering.
 * @param projection Projection matrix.
 */
void Scene::RenderHeldEntity(Shader* shader, glm::mat4 projection)
{
    RenderLogic(shader, projection);

    Entity* heldEntity = player->getHeldEntity();
    if (heldEntity->getModel() != nullptr)
    {
        if (heldEntity->getTitle() == "Flashlight") return;

        glClear(GL_DEPTH_BUFFER_BIT);
        heldEntity->DrawEntity(shader);
    }
}
