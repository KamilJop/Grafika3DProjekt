#pragma once
#define NOMINMAX
#include <Windows.h>
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

/**
 * @class Scene
 * @brief Manages all world entities, lights, rendering passes, and player interactions.
 *
 * The Scene class handles:
 * - Entity storage and updates
 * - Lighting (directional, point, flashlight)
 * - Rendering (normal, outlined, shadow maps, held items)
 * - Tooltip rendering
 * - Player physics and interaction updates
 */
class Scene
{
public:

    /**
     * @brief Constructs a new Scene object.
     *
     * @param cam Pointer to the main camera.
     * @param play Pointer to the player object.
     * @param renderer Primary text renderer (large text).
     * @param secondRenderer Secondary text renderer (small text).
     */
    Scene(Camera* cam, Player* play, TextRenderer* renderer, TextRenderer* secondRenderer);

    /**
     * @brief Scene destructor.
     */
    ~Scene();

    /**
     * @brief Adds an entity to the scene.
     *
     * @param entity Pointer to the entity to add.
     */
    void AddEntity(Entity* entity)
    {
        entities.push_back(entity);
    }

    /**
     * @brief Adds a point light to the scene.
     *
     * @param light Pointer to the point light.
     */
    void AddPointLight(PointLight* light)
    {
        pointLights.push_back(light);
    }

    /**
     * @brief Sets the directional light for the scene.
     *
     * @param light Pointer to the directional light.
     */
    void SetDirectionalLight(DirectionalLight* light)
    {
        dirLight = light;
    }

    /**
     * @brief Sets the player's flashlight light source.
     *
     * @param light Pointer to the flashlight light.
     */
    void SetFlashlight(Flashlight* light)
    {
        flashLight = light;
    }

    /**
     * @brief Gets the scene's camera.
     *
     * @return Pointer to the camera.
     */
    Camera* GetCamera()
    {
        return camera;
    }

    /**
     * @brief Renders all outlined entities and their tooltips.
     *
     * @param shader Shader used for rendering.
     * @param projection Projection matrix.
     * @param w Screen width.
     * @param h Screen height.
     */
    void RenderWithOutline(Shader* shader, glm::mat4 projection, int w, int h);

    /**
     * @brief Renders all non‑outlined entities.
     *
     * @param shader Shader used for rendering.
     * @param projection Projection matrix.
     */
    void RenderWithoutOutline(Shader* shader, glm::mat4 projection);

    /**
     * @brief Renders the flashlight entity in first‑person view.
     *
     * @param shader Shader used for rendering.
     * @param projection Projection matrix.
     */
    void RenderFlashlightEntity(Shader* shader, glm::mat4 projection);

    /**
     * @brief Renders the held entity (weapon/item) in first‑person view.
     *
     * @param shader Shader used for rendering.
     * @param projection Projection matrix.
     */
    void RenderHeldEntity(Shader* shader, glm::mat4 projection);

    /**
     * @brief Updates the scene each frame.
     *
     * Handles:
     * - Player physics
     * - Flashlight updates
     * - Entity updates and destruction
     * - Targeted entity detection
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime);

    /**
     * @brief Renders all shadow‑casting entities into the shadow map.
     *
     * @param shadowShader Shader used for shadow rendering.
     */
    void RenderShadowMap(Shader* shadowShader);

    /**
     * @brief Renders tooltip text above an outlined entity.
     *
     * @param selectedEntity Entity being hovered.
     * @param w Screen width.
     * @param h Screen height.
     */
    void RenderTooltip(Entity* selectedEntity, int w, int h);

    /**
     * @brief Gets all entities in the scene.
     *
     * @return Vector of entity pointers.
     */
    std::vector<Entity*> getEntities() { return entities; }

    /**
     * @brief Gets all point lights in the scene.
     *
     * @return Vector of point light pointers.
     */
    std::vector<PointLight*> getPointLights() { return pointLights; }

private:

    /** @brief All entities currently in the scene. */
    std::vector<Entity*> entities;

    /** @brief All point lights in the scene. */
    std::vector<PointLight*> pointLights;

    /** @brief Directional light source. */
    DirectionalLight* dirLight;

    /** @brief Flashlight light source. */
    Flashlight* flashLight;

    /** @brief Main camera used for rendering. */
    Camera* camera;

    /** @brief Player object associated with the scene. */
    Player* player;

    /** @brief Primary text renderer (large text). */
    TextRenderer* textRenderer;

    /** @brief Secondary text renderer (smaller text). */
    TextRenderer* smallerTextRenderer;

    /** @brief Global configuration reference. */
    Config& config = Config::getInstance();

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
    void RenderLogic(Shader* shader, glm::mat4 projection);

    /**
     * @brief Finds an entity by its title.
     *
     * @param title Title to search for.
     * @return Pointer to entity or nullptr if not found.
     */
    Entity* getEntityByTitle(const std::string& title);
};
