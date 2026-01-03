#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

/**
 * @class Key
 * @brief Represents a pickable key item used to unlock doors, drawers, or other locked entities.
 *
 * A Key:
 * - Has a unique tag used to match it with locks
 * - Can be picked up by the player
 * - Plays a pickup sound
 * - Stores a texture for inventory display
 */
class Key : public Entity
{
public:

    /**
     * @brief Constructs a new Key entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Unique key tag
     * - Inventory texture
     * - Pickup sound effect
     * - Marks the key as pickable
     *
     * @param model Pointer to the 3D model of the key.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param tag Unique identifier used to match this key with locks.
     * @param tex Texture used when displaying the key in the inventory.
     * @param interaction Whether the key is interactable (usually true).
     */
    Key(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
        std::string tag, Texture* tex, bool interaction = true);

    /**
     * @brief Handles interaction with the key.
     *
     * Behavior:
     * - Plays pickup sound
     * - Marks the key as picked up
     * - Displays a short subtitle hint
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the key each frame.
     *
     * Once the key is picked up, it is flagged for destruction so the world
     * no longer renders it.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the texture used for inventory display.
     *
     * @return Pointer to the key's texture.
     */
    Texture* getTexture() override { return itemTexture; }

    /**
     * @brief Gets the action text displayed when targeting the key.
     *
     * @return "Pick Up"
     */
    std::string GetActionText() override { return "Pick Up"; }

    /**
     * @brief Key destructor.
     */
    ~Key();

private:

    /** @brief Whether the key has been picked up. */
    bool isPickedUp = false;

    /** @brief Texture used when displaying the key in the inventory. */
    Texture* itemTexture;

    /** @brief Path to the key pickup sound effect. */
    std::string pickupSoundPath = "Audio/key_pickup.mp3";

    /** @brief Sound effect name used for pickup. */
    std::string pickupSoundName = "key_pickup";
};
