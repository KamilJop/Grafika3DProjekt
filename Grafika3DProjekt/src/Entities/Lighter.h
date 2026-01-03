#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"

/**
 * @class Lighter
 * @brief Represents a simple pickable lighter item.
 *
 * The Lighter:
 * - Can be picked up by the player
 * - Stores an inventory texture
 * - Uses the tag "Lighter" for puzzle interactions
 * - Destroys itself after being picked up
 */
class Lighter : public Entity
{
public:

    /**
     * @brief Constructs a new Lighter entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Marks the lighter as pickable
     * - Stores the inventory texture
     * - Assigns the item tag "Lighter"
     *
     * @param model Pointer to the lighter's 3D model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param tex Texture used for inventory display.
     * @param interaction Whether the lighter is interactable.
     */
    Lighter(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
            Texture* tex, bool interaction = true);

    /**
     * @brief Handles interaction with the lighter.
     *
     * Behavior:
     * - Marks the lighter as picked up
     * - Displays a short subtitle hint
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the lighter each frame.
     *
     * Once picked up, the lighter is flagged for destruction so it no longer
     * appears in the world.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the texture used for inventory display.
     *
     * @return Pointer to the lighter's texture.
     */
    Texture* getTexture() override { return itemTexture; }

    /**
     * @brief Gets the action text displayed when targeting the lighter.
     *
     * @return "Pick Up"
     */
    std::string GetActionText() override { return "Pick Up"; }

    /**
     * @brief Lighter destructor.
     */
    ~Lighter();

private:

    /** @brief Whether the lighter has been picked up. */
    bool isPickedUp = false;

    /** @brief Texture used when displaying the lighter in the inventory. */
    Texture* itemTexture;
};
