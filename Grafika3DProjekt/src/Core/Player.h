#pragma once
#define NOMINMAX
#include <Windows.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "Entities/Entity.h"
#include "Rendering/CollisionBox.h"
#include <vector>
#include "Inventory.h"

/**
 * @class Player
 * @brief Handles player movement, physics, interaction, inventory, and held items.
 *
 * The Player class manages:
 * - Physics and collision resolution
 * - Camera height and crouching
 * - Flashlight state
 * - Raycasting for interactable entities
 * - Inventory and item pickup
 * - Held entity (e.g., weapon or item in hand)
 */
class Player
{
public:

    /**
     * @brief Constructs a new Player object.
     *
     * @param cam Pointer to the player's camera.
     * @param flashlight Pointer to the flashlight entity (ignored in collisions).
     * @param pos Initial world position of the player.
     * @param vel Initial velocity of the player.
     */
    Player(Camera* cam, Entity* flashlight,
           glm::vec3 pos = glm::vec3(-3.0f, 1.0f, 0.0f),
           glm::vec3 vel = glm::vec3(0.0f));

    /**
     * @brief Destructor for Player.
     */
    ~Player();

    /**
     * @brief Updates player physics, gravity, collisions, and camera height.
     *
     * @param deltaTime Time elapsed since last frame.
     * @param entities List of world entities to check collisions against.
     */
    void UpdatePhysics(float deltaTime, std::vector<Entity*>& entities);

    /**
     * @brief Performs raycast to detect which entity the player is looking at.
     *
     * Highlights the closest interactable entity within reach.
     *
     * @param entities List of world entities.
     */
    void checkTargettedEntity(std::vector<Entity*>& entities);

    /**
     * @brief Checks ray–AABB intersection for entity selection.
     *
     * @param rayOrigin Starting point of the ray.
     * @param rayDirection Normalized direction of the ray.
     * @param entity Entity to test against.
     * @param outDistance Output distance to intersection point.
     * @return true If ray intersects the entity.
     * @return false Otherwise.
     */
    bool checkRayEntityIntersection(const glm::vec3& rayOrigin,
                                    const glm::vec3& rayDirection,
                                    Entity* entity,
                                    float& outDistance);

    /**
     * @brief Makes the player jump if grounded.
     */
    void Jump();

    /**
     * @brief Enables or disables the flashlight.
     *
     * @param state True to turn on, false to turn off.
     */
    void changeFlashlightState(bool state);

    /**
     * @brief Enables or disables crouching.
     *
     * @param state True to crouch, false to stand.
     */
    void Crouch(bool state);

    /**
     * @brief Gets the current flashlight state.
     *
     * @return True if flashlight is on.
     */
    bool getFlashlightState() { return isFlashlightOn; }

    /**
     * @brief Gets the player's inventory.
     *
     * @return Pointer to Inventory object.
     */
    Inventory* getInventory() { return &playerInventory; }

    /**
     * @brief Updates the player's collision bounding box.
     */
    void updatePlayerCollisions();

    /**
     * @brief Sets the model of the held entity.
     *
     * @param model Pointer to the model.
     */
    void setHeldEntityModel(Model* model) { heldEntity->setModel(model); }

    /**
     * @brief Sets the scale of the held entity.
     *
     * @param scale New scale vector.
     */
    void setHeldEntityScale(glm::vec3 scale) { heldEntity->setScale(scale); }

    /**
     * @brief Sets the walk timer used for bobbing animation.
     *
     * @param time New walk timer value.
     */
    void setWalkTimer(float time) { walkTimer = time; }

    /**
     * @brief Gets the current walk timer.
     *
     * @return Walk timer value.
     */
    float getWalkTimer() { return walkTimer; }

    /**
     * @brief Returns whether the player is crouching.
     *
     * @return True if crouching.
     */
    bool getCrouching() { return isCrouching; }

    /**
     * @brief Gets the held entity (weapon/item).
     *
     * @return Pointer to held Entity.
     */
    Entity* getHeldEntity() { return heldEntity; }

    /**
     * @brief Updates the position and rotation of the held entity.
     *
     * @param finalPos Target world position.
     */
    void updateHeldEntityPosition(const glm::vec3& finalPos);

    /**
     * @brief Picks up an interactable entity and adds it to inventory.
     *
     * @param entity Entity to pick up.
     */
    void pickUpEntity(Entity* entity);

    /**
     * @brief Gets the currently targeted entity.
     *
     * @return Pointer to targeted entity or nullptr.
     */
    Entity* getTargettedEntity() { return targettedEntity; }

    /**
     * @brief Checks if the player is targeting an entity.
     *
     * @return True if an entity is targeted.
     */
    bool hasTargettedEntity() { return targettedEntity != nullptr; }

    /**
     * @brief Sets the player's velocity.
     *
     * @param vel New velocity vector.
     */
    void setVelocity(glm::vec3 vel) { velocity = vel; }

    /**
     * @brief Sets X component of velocity.
     */
    void setVelocityX(float x) { velocity.x = x; }

    /**
     * @brief Sets Y component of velocity.
     */
    void setVelocityY(float y) { velocity.y = y; }

    /**
     * @brief Sets Z component of velocity.
     */
    void setVelocityZ(float z) { velocity.z = z; }

    /**
     * @brief Gets the player's velocity.
     *
     * @return Velocity vector.
     */
    glm::vec3 getVelocity() { return velocity; }

    /**
     * @brief Gets the player's collision box.
     *
     * @return CollisionBox representing player bounds.
     */
    CollisionBox GetPlayerCollisions() { return playerCollisions; }

private:

    /** @brief Player world position. */
    glm::vec3 position;

    /** @brief Player velocity vector. */
    glm::vec3 velocity;

    /** @brief Flashlight entity (ignored in collisions). */
    Entity* flashlightEntity;

    /** @brief Entity held by the player (weapon/item). */
    Entity* heldEntity;

    /** @brief Entity currently targeted by raycast. */
    Entity* targettedEntity = nullptr;

    /** @brief Timer used for walking bob animation. */
    float walkTimer = 0.0f;

    /** @brief Whether the flashlight is currently on. */
    bool isFlashlightOn = false;

    /** @brief Whether the player is standing on ground. */
    bool onGround = false;

    /** @brief Whether the player is crouching. */
    bool isCrouching = false;

    /** @brief Pointer to the player's camera. */
    Camera* camera;

    /** @brief Player collision bounding box. */
    CollisionBox playerCollisions;

    /** @brief Player inventory system. */
    Inventory playerInventory;
};
