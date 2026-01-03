#include "Lighter.h"

/**
 * @brief Constructs a new Lighter entity.
 *
 * A Lighter is a simple pickable item used to ignite candles or other
 * interactable objects. This constructor:
 * - Marks the lighter as pickable
 * - Stores its inventory texture
 * - Assigns the item tag "Lighter"
 *
 * @param model Pointer to the lighter's 3D model.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param tex Texture used for inventory display.
 * @param interaction Whether the lighter is interactable.
 */
Lighter::Lighter(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                 Texture* tex, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    pickable = true;
    isPickedUp = false;
    itemTexture = tex;
    itemTag = "Lighter";
}

/**
 * @brief Handles interaction with the lighter.
 *
 * Behavior:
 * - Marks the lighter as picked up
 * - Displays a short subtitle hint
 *
 * The actual inventory insertion is handled externally.
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Lighter::Interact(Inventory* playerInventory)
{
    isPickedUp = true;
    UI::SetSubtitle("Let's burn this place.", 2.0f);
}

/**
 * @brief Updates the lighter each frame.
 *
 * Once picked up, the lighter is flagged for destruction so it no longer
 * appears in the world.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Lighter::Update(float deltaTime)
{
    if (isPickedUp)
    {
        shouldGetDestroyed = true;
    }
}
