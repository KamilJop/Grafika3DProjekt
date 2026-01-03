#include "Readable.h"

/**
 * @brief Constructs a new Readable entity.
 *
 * A Readable object represents any world item the player can inspect more closely,
 * such as notes, letters, books, or documents. When interacted with, the readable
 * displays a full‑screen texture using a SpriteRenderer.
 *
 * This constructor:
 * - Initializes the base Entity
 * - Stores the renderer and page texture
 * - Sets the action text to "Read"
 * - Loads the page‑flip sound effect
 *
 * @param model Pointer to the 3D model of the readable object.
 * @param position World position.
 * @param rotation Rotation vector.
 * @param scale Scale vector.
 * @param spriteRenderer Renderer used to draw the readable page on screen.
 * @param texture Texture displayed when reading.
 * @param interaction Whether the readable is interactable.
 */
Readable::Readable(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
                   SpriteRenderer* spriteRenderer, Texture* texture, bool interaction)
    : Entity(model, position, rotation, scale, interaction),
      renderer(spriteRenderer),
      pageTexture(texture)
{
    actionText = "Read";
    AudioManager::GetInstance().Load2DSoundEffect("page_flip", readingSoundPath);
}

/**
 * @brief Handles interaction with the readable object.
 *
 * Behavior:
 * - Sets the readable into "reading mode"
 * - Plays a page‑flip sound effect
 *
 * The actual rendering of the readable page is handled externally
 * (likely in the UI or game state manager).
 *
 * @param playerInventory Pointer to the player's inventory (unused).
 */
void Readable::Interact(Inventory* playerInventory)
{
    isReading = true;
    AudioManager::GetInstance().Play2DSoundEffect("page_flip", Config::getInstance().sfxVolume);
}
