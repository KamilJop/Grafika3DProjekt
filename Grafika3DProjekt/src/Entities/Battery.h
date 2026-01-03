#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

/**
 * @class Battery
 * @brief Represents a pickable battery item in the world.
 *
 * The Battery class extends Entity and provides:
 * - Pickup interaction
 * - Custom pickup sound
 * - Automatic destruction after being collected
 * - Texture access for inventory display
 */
class Battery : public Entity
{
public:

    /**
     * @brief Constructs a new Battery item.
     *
     * @param model Pointer to the 3D model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param tag Item identifier tag.
     * @param tex Pointer to the battery texture.
     * @param interaction Whether the item is interactable.
     */
    Battery(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
            std::string tag, Texture* tex, bool interaction = true);

    /**
     * @brief Called when the player interacts with the battery.
     *
     * Plays a pickup sound, displays a UI message, and marks the item as collected.
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the battery each frame.
     *
     * If picked up, the battery is flagged for destruction.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the texture associated with the battery.
     *
     * @return Pointer to the texture.
     */
    Texture* getTexture() override { return itemTexture; }

    /**
     * @brief Gets the action text displayed when targeting the battery.
     *
     * @return A string representing the action ("Pick Up").
     */
    std::string GetActionText() override { return "Pick Up"; }

    /**
     * @brief Battery destructor.
     */
    ~Battery();

private:

    /** @brief Whether the battery has been picked up. */
    bool isPickedUp = false;

    /** @brief Texture used for inventory display. */
    Texture* itemTexture;

    /** @brief Path to the pickup sound effect. */
    std::string pickupSoundPath = "Audio/item_pick.mp3";
};
