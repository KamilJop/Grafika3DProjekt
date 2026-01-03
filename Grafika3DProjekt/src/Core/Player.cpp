#include "Player.h"

/**
 * @brief Constructs a new Player object.
 *
 * Initializes camera reference, flashlight entity, position, velocity,
 * inventory, and creates a held entity used for first‑person item rendering.
 *
 * @param cam Pointer to the player's camera.
 * @param flashlight Pointer to the flashlight entity (ignored in collisions).
 * @param pos Initial world position of the player.
 * @param vel Initial velocity of the player.
 */
Player::Player(Camera* cam, Entity* flashlight, glm::vec3 pos, glm::vec3 vel)
{
    camera = cam;
    flashlightEntity = flashlight;
    position = pos;
    velocity = vel;
    playerInventory = Inventory();

    // Held entity (e.g., weapon or item in hand)
    heldEntity = new Entity(nullptr, glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    heldEntity->setCastsShadow(false);
    heldEntity->setColissions(false);

    updatePlayerCollisions();
}

/**
 * @brief Player destructor.
 *
 * Clears references and resets basic vectors.
 */
Player::~Player()
{
    camera = nullptr;
    position = glm::vec3(0.0f);
    velocity = glm::vec3(0.0f);
}

/**
 * @brief Updates player physics, movement, gravity, and collision handling.
 *
 * Handles:
 * - Gravity
 * - Movement on X, Y, Z axes
 * - Collision resolution with world entities
 * - Ground detection
 * - Camera height smoothing (standing/crouching)
 *
 * @param deltaTime Time elapsed since last frame.
 * @param entities List of all world entities to check collisions against.
 */
void Player::UpdatePhysics(float deltaTime, std::vector<Entity*>& entities)
{
    // Remember previous position for collision rollback
    float previousX = position.x;
    float previousY = position.y;
    float previousZ = position.z;

    // Apply gravity
    const float gravity = -9.81f;
    if (!onGround)
        velocity.y += gravity * deltaTime;

    // -------------------------
    // X AXIS MOVEMENT
    // -------------------------
    position.x += velocity.x * deltaTime;
    updatePlayerCollisions();

    for (auto& entity : entities)
    {
        if (entity == flashlightEntity) continue;
        if (!entity->getColissions()) continue;

        CollisionBox otherBox = entity->GetCollisions();

        if (playerCollisions.min.x < otherBox.max.x && playerCollisions.max.x > otherBox.min.x &&
            playerCollisions.min.y < otherBox.max.y && playerCollisions.max.y > otherBox.min.y &&
            playerCollisions.min.z < otherBox.max.z && playerCollisions.max.z > otherBox.min.z)
        {
            position.x = previousX;
            velocity.x = 0.0f;
            updatePlayerCollisions();
            break;
        }
    }

    // -------------------------
    // Z AXIS MOVEMENT
    // -------------------------
    position.z += velocity.z * deltaTime;
    updatePlayerCollisions();

    for (auto& entity : entities)
    {
        if (entity == flashlightEntity) continue;
        if (!entity->getColissions()) continue;

        CollisionBox otherBox = entity->GetCollisions();

        if (playerCollisions.min.x < otherBox.max.x && playerCollisions.max.x > otherBox.min.x &&
            playerCollisions.min.y < otherBox.max.y && playerCollisions.max.y > otherBox.min.y &&
            playerCollisions.min.z < otherBox.max.z && playerCollisions.max.z > otherBox.min.z)
        {
            position.z = previousZ;
            velocity.z = 0.0f;
            updatePlayerCollisions();
            break;
        }
    }

    // -------------------------
    // Y AXIS MOVEMENT (gravity)
    // -------------------------
    position.y += velocity.y * deltaTime;
    updatePlayerCollisions();

    bool onObject = false;

    for (auto& entity : entities)
    {
        if (entity == flashlightEntity) continue;
        if (!entity->getColissions()) continue;

        CollisionBox otherBox = entity->GetCollisions();

        if (playerCollisions.min.x < otherBox.max.x && playerCollisions.max.x > otherBox.min.x &&
            playerCollisions.min.y < otherBox.max.y && playerCollisions.max.y > otherBox.min.y &&
            playerCollisions.min.z < otherBox.max.z && playerCollisions.max.z > otherBox.min.z)
        {
            if (velocity.y > 0)
            {
                // Hitting ceiling
                position.y = previousY;
                velocity.y = 0.0f;
            }
            else
            {
                // Standing on object
                onObject = true;
                position.y = previousY;
                velocity.y = 0.0f;
            }

            updatePlayerCollisions();
            break;
        }
    }

    onGround = onObject;

    // Smooth camera height (standing/crouching)
    float targetHeight = isCrouching ? 1.0f : 1.5f;
    float currentHeight = camera->getCameraPosition().y - position.y;
    float newHeight = glm::mix(currentHeight, targetHeight, 10.0f * deltaTime);

    camera->setPosition(position + glm::vec3(0.0f, newHeight, 0.0f));
}

/**
 * @brief Makes the player jump if grounded.
 */
void Player::Jump()
{
    if (onGround)
    {
        velocity.y = 6.0f;
        onGround = false;
    }
}

/**
 * @brief Enables or disables crouching.
 *
 * @param state True to crouch, false to stand.
 */
void Player::Crouch(bool state)
{
    isCrouching = state;
}

/**
 * @brief Turns the flashlight on or off.
 *
 * @param state True to enable flashlight, false to disable.
 */
void Player::changeFlashlightState(bool state)
{
    isFlashlightOn = state;
}

/**
 * @brief Updates the position and rotation of the held entity (weapon/item).
 *
 * Applies walking bobbing effect and aligns rotation with camera.
 *
 * @param finalPos Target world position for the held entity.
 */
void Player::updateHeldEntityPosition(const glm::vec3& finalPos)
{
    glm::vec3 finalPosAdjusted = finalPos;

    if (walkTimer > 0.0f)
    {
        float bobFrequency = 10.0f;
        float bobAmountX = 0.015f;
        float bobAmountY = 0.005f;

        float bobX = cos(walkTimer * bobFrequency / 2.0f) * bobAmountX;
        float bobY = sin(walkTimer * bobFrequency) * bobAmountY;

        finalPosAdjusted += camera->getCameraRight() * bobX;
        finalPosAdjusted += camera->getCameraUp() * bobY;
    }

    heldEntity->setPosition(finalPosAdjusted);
    heldEntity->setRotation(glm::vec3(-camera->getPitch(), -camera->getYaw() + 90.0f, 0.0f));
}

/**
 * @brief Updates the player's collision bounding box based on position and stance.
 */
void Player::updatePlayerCollisions()
{
    float width = 0.7f;
    float height = isCrouching ? 1.4f : 1.8f;

    float halfWidth = width / 2.0f;

    playerCollisions.min = glm::vec3(position.x - halfWidth, position.y, position.z - halfWidth);
    playerCollisions.max = glm::vec3(position.x + halfWidth, position.y + height, position.z + halfWidth);
}

/**
 * @brief Performs raycast to detect which entity the player is looking at.
 *
 * Highlights the closest interactable entity within reach.
 *
 * @param entities List of world entities.
 */
void Player::checkTargettedEntity(std::vector<Entity*>& entities)
{
    float reachDistance = 3.0f;
    glm::vec3 rayOrigin = camera->getCameraPosition();
    glm::vec3 rayDirection = camera->getCameraFront();

    Entity* closestEntity = nullptr;
    targettedEntity = nullptr;
    float closestDistance = reachDistance;

    for (auto& entity : entities)
    {
        if (entity == flashlightEntity) continue;
        if (!entity->getInteractable()) continue;

        entity->setOutlined(false);

        float intersectionDistance = 10000.0f;

        if (checkRayEntityIntersection(rayOrigin, rayDirection, entity, intersectionDistance))
        {
            if (intersectionDistance < closestDistance)
            {
                closestDistance = intersectionDistance;
                closestEntity = entity;
            }
        }
    }

    if (closestEntity)
    {
        closestEntity->setOutlined(true);
        targettedEntity = closestEntity;
    }
}

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
bool Player::checkRayEntityIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, Entity* entity, float& outDistance)
{
    CollisionBox box = entity->GetCollisions();

    float tx1 = (box.min.x - rayOrigin.x) / rayDirection.x;
    float tx2 = (box.max.x - rayOrigin.x) / rayDirection.x;

    float tmin = std::min(tx1, tx2);
    float tmax = std::max(tx1, tx2);

    float ty1 = (box.min.y - rayOrigin.y) / rayDirection.y;
    float ty2 = (box.max.y - rayOrigin.y) / rayDirection.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    float tz1 = (box.min.z - rayOrigin.z) / rayDirection.z;
    float tz2 = (box.max.z - rayOrigin.z) / rayDirection.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    if (tmax >= tmin && tmin >= 0.0f)
    {
        outDistance = tmin;
        return true;
    }

    return false;
}

/**
 * @brief Picks up an interactable entity and adds it to the inventory.
 *
 * Marks the entity for destruction after pickup.
 *
 * @param entity Entity to pick up.
 */
void Player::pickUpEntity(Entity* entity)
{
    if (entity->getInteractable())
    {
        playerInventory.AddItem(
            entity->getTag(),
            entity->getTitle(),
            entity->getTexture(),
            entity->getModel(),
            entity->getScale()
        );

        entity->setShouldGetDestroyed(true);
    }
}
