#pragma once
#include "Entity.h"
#include "../Rendering/Texture.h"

/**
 * @class Pickable
 * @brief Represents a generic pickable item in the world.
 *
 * A Pickable object:
 * - Can be picked up by the player
 * - Stores an inventory texture
 * - Plays a pickup sound
 * - Removes itself from the world after being collected
 *
 * This class is used for simple items that do not require special logic
 * beyond being collectible.
 */
class Pickable : public Entity
{
public:

    /**
     * @brief Constructs a new Pickable entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Item tag and inventory texture
     * - Marks the object as pickable
     * - Loads the pickup sound effect
     *
     * @param model Pointer to the item's 3D model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param tag Unique identifier for the item.
     * @param tex Texture used for inventory display.
     * @param interaction Whether the item is interactable.
     */
    Pickable(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
             std::string tag, Texture* tex, bool interaction = true);

    /**
     * @brief Handles interaction with the pickable item.
     *
     * Behavior:
     * - Plays pickup sound
     * - Marks the item as picked up
     * - Displays a short subtitle
     *
     * The actual inventory insertion is handled externally.
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the pickable item each frame.
     *
     * Once picked up, the item is flagged for destruction so it no longer
     * appears in the world.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the texture used for inventory display.
     *
     * @return Pointer to the item's texture.
     */
    Texture* getTexture() override { return itemTexture; }

    /**
     * @brief Gets the action text displayed when targeting the item.
     *
     * @return "Pick Up"
     */
    std::string GetActionText() override { return "Pick Up"; }

    /**
     * @brief Pickable destructor.
     */
    ~Pickable();

private:

    /** @brief Whether the item has been picked up. */
    bool isPickedUp = false;

    /** @brief Texture used when displaying the item in the inventory. */
    Texture* itemTexture;

    /** @brief Path to the pickup sound effect. */
    std::string pickupSoundPath = "Audio/item_pick.mp3";
};
