#include "Pickable.h"

/**
 * @brief Constructs a new Pickable entity.
 *
 * A Pickable object is a generic item that the player can pick up.
 * This constructor:
 * - Assigns the item tag
 * - Stores the inventory texture
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
Pickable::Pickable(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                   std::string tag, Texture* tex, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    itemTag = tag;
    itemTexture = tex;
    pickable = true;

    AudioManager::GetInstance().Load3DSoundEffect(pickupSoundPath, pickupSoundPath);
}

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
void Pickable::Interact(Inventory* playerInventory)
{
    AudioManager::GetInstance().Play3DSoundEffect(pickupSoundPath, position, 1.0f);
    isPickedUp = true;

    UI::SetSubtitle("Item picked up.", 2.0f);
}

/**
 * @brief Updates the pickable item each frame.
 *
 * Once picked up, the item is flagged for destruction so it no longer
 * appears in the world.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Pickable::Update(float deltaTime)
{
    if (isPickedUp)
    {
        shouldGetDestroyed = true;
    }
}

/**
 * @brief Pickable destructor.
 *
 * Clears texture reference and resets pickup state.
 */
Pickable::~Pickable()
{
    itemTexture = nullptr;
    isPickedUp = false;
}
