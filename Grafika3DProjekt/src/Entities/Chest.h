#pragma once

#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Core/Scene.h"

/**
 * @class Chest
 * @brief Represents an interactable chest composed of a lower body and a movable lid.
 *
 * The Chest class:
 * - Contains two separate Entity objects (base + lid)
 * - Supports locked/unlocked interaction logic
 * - Plays an opening sound when unlocked
 * - Animates the lid opening over time
 */
class Chest : public Entity
{
public:

    /**
     * @brief Constructs a new Chest entity.
     *
     * Initializes:
     * - Base chest entity (this object)
     * - Upper lid entity, positioned with an offset
     * - Registers the lid inside the scene
     * - Loads the chest opening sound
     *
     * @param lowerPart Pointer to the model for the lower chest body.
     * @param upperPart Pointer to the model for the chest lid.
     * @param pos World position of the chest.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param scene Pointer to the scene so the lid can be added as an entity.
     * @param interaction Whether the chest is interactable.
     */
    Chest(Model* lowerPart, Model* upperPart, glm::vec3 pos, glm::vec3 rot,
          glm::vec3 scal, Scene* scene, bool interaction = true);

    /**
     * @brief Chest destructor.
     */
    ~Chest();

    /**
     * @brief Handles player interaction with the chest.
     *
     * Behavior:
     * - If locked: shows a subtitle once and prevents opening
     * - If unlocked: plays opening sound and begins lid animation
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the chest each frame.
     *
     * Handles:
     * - Lid opening animation
     * - Disabling interaction and outline once opening begins
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Unlocks the chest, allowing it to be opened.
     */
    void OpenChest() { isLocked = false; }

private:

    /** @brief Pointer to the upper lid entity. */
    Entity* upperPart;

    /** @brief Whether the player has already attempted to open the locked chest. */
    bool triedToOpen = false;

    /** @brief Whether the chest is currently locked. */
    bool isLocked = true;

    /** @brief Whether the chest has been opened. */
    bool isOpen = false;

    /** @brief Target rotation angle for the lid when fully opened. */
    float targetRotation = 0.0f;

    /** @brief Path to the chest opening sound effect. */
    std::string chestOpenSound = "Audio/chest_opening.mp3";
};
