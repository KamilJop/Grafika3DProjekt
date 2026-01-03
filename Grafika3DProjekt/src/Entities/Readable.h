#pragma once
#include "Entity.h"
#include "../Systems/SpriteRenderer.h"

/**
 * @class Readable
 * @brief Represents an interactable object that the player can read.
 *
 * A Readable object is used for notes, letters, documents, or any item
 * that displays a full‑screen texture when examined. When interacted with,
 * the readable enters a "reading mode" and plays a page‑flip sound.
 *
 * Rendering of the readable page is handled externally via a SpriteRenderer.
 */
class Readable : public Entity
{
public:

    /**
     * @brief Constructs a new Readable entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Stores the SpriteRenderer used to draw the readable page
     * - Stores the texture representing the readable content
     * - Loads the page‑flip sound effect
     *
     * @param model Pointer to the readable object's 3D model.
     * @param position World position.
     * @param rotation Rotation vector.
     * @param scale Scale vector.
     * @param spriteRenderer Renderer used to draw the readable page on screen.
     * @param texture Texture displayed when reading.
     * @param interaction Whether the readable is interactable.
     */
    Readable(Model* model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale,
             SpriteRenderer* spriteRenderer, Texture* texture, bool interaction);

    /**
     * @brief Handles interaction with the readable object.
     *
     * Behavior:
     * - Sets the readable into "reading mode"
     * - Plays a page‑flip sound effect
     *
     * The actual rendering of the readable page is handled outside this class.
     *
     * @param playerInventory Pointer to the player's inventory (unused).
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the readable each frame.
     *
     * Currently unused — readable logic is handled in Interact() and externally.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override {};

    /**
     * @brief Sets whether the readable is currently being viewed.
     *
     * @param state True if the player is reading the item, false otherwise.
     */
    void setReadingState(bool state) { isReading = state; }

    /**
     * @brief Gets the texture displayed when reading the item.
     *
     * @return Pointer to the readable's page texture.
     */
    Texture* getPageTexture() { return pageTexture; }

private:

    /** @brief Renderer used to draw the readable page on screen. */
    SpriteRenderer* renderer;

    /** @brief Texture displayed when the player reads the item. */
    Texture* pageTexture;

    /** @brief Whether the readable is currently being viewed. */
    bool isReading = false;

    /** @brief Path to the page‑flip sound effect. */
    std::string readingSoundPath = "Audio/page_flip.mp3";
};
