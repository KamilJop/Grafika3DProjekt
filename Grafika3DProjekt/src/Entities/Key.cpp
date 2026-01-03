#include "Key.h"

/**
 * @brief Constructs a new Key entity.
 *
 * A Key is a pickable item used to unlock doors, drawers, or other interactable
 * objects. It stores:
 * - A unique item tag (used for matching with locks)
 * - A texture for UI inventory display
 * - A pickup sound effect
 *
 * @param model Pointer to the 3D model of the key.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param tag Unique identifier used to match this key with locks.
 * @param tex Texture used when displaying the key in the inventory.
 * @param interaction Whether the key is interactable (usually true).
 */
Key::Key(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
         std::string tag, Texture* tex, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    itemTag = tag;
    itemTexture = tex;
    pickable = true;

    AudioManager::GetInstance().Load3DSoundEffect(pickupSoundName, pickupSoundPath);
}

/**
 * @brief Handles interaction with the key.
 *
 * Behavior:
 * - Plays pickup sound
 * - Marks the key as picked up
 * - Displays a short subtitle hint
 *
 * The actual inventory insertion is handled externally.
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Key::Interact(Inventory* playerInventory)
{
    AudioManager::GetInstance().Play3DSoundEffect(pickupSoundName, position, 1.0f);
    isPickedUp = true;

    UI::SetSubtitle("I wonder what this opens.", 2.0f);
}

/**
 * @brief Updates the key each frame.
 *
 * Once the key is picked up, it is flagged for destruction so the world
 * no longer renders it.
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Key::Update(float deltaTime)
{
    if (isPickedUp)
    {
        shouldGetDestroyed = true;
    }
}

/**
 * @brief Key destructor.
 *
 * Clears texture reference and resets pickup state.
 */
Key::~Key()
{
    itemTexture = nullptr;
    isPickedUp = false;
}
