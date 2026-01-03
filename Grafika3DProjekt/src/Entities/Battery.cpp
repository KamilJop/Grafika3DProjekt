#include "Battery.h"

/**
 * @brief Constructs a new Battery item entity.
 *
 * Initializes:
 * - Base Entity properties (model, transform, interaction flag)
 * - Item tag and texture
 * - Marks the item as pickable
 * - Loads the 3D pickup sound effect
 *
 * @param model Pointer to the 3D model.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param tag Item identifier tag.
 * @param tex Pointer to the item's texture.
 * @param interaction Whether the item is interactable.
 */
Battery::Battery(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                 std::string tag, Texture* tex, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    itemTag = tag;
    itemTexture = tex;
    pickable = true;

    AudioManager::GetInstance().Load3DSoundEffect(pickupSoundPath, pickupSoundPath);
}

/**
 * @brief Called when the player interacts with the battery.
 *
 * Plays a pickup sound, marks the battery as picked up,
 * and displays a UI subtitle message.
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Battery::Interact(Inventory* playerInventory)
{
    AudioManager::GetInstance().Play3DSoundEffect(pickupSoundPath, position, 1.0f);
    isPickedUp = true;

    UI::SetSubtitle("A battery. Nice!", 2.0f);
}

/**
 * @brief Updates the battery each frame.
 *
 * If the battery has been picked up, it is flagged for destruction.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Battery::Update(float deltaTime)
{
    if (isPickedUp)
    {
        shouldGetDestroyed = true;
    }
}

/**
 * @brief Battery destructor.
 *
 * Clears texture reference and resets pickup state.
 */
Battery::~Battery()
{
    itemTexture = nullptr;
    isPickedUp = false;
}
