#pragma once

#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Core/Scene.h"

/**
 * @class Drawer
 * @brief Represents an interactable drawer that can be locked, unlocked, opened, and closed.
 *
 * The Drawer class provides:
 * - Lock/unlock logic using key tags
 * - Smooth opening/closing movement
 * - Shaking animation when trying to open a locked drawer
 * - Optional contained entity that moves with the drawer
 * - Sound effects for opening, closing, and locked attempts
 */
class Drawer : public Entity
{
public:

    /**
     * @brief Constructs a new Drawer entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Original drawer position
     * - Key tag required to unlock the drawer
     * - Loads all drawer-related sound effects
     *
     * @param model Pointer to the drawer model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param keyTag Tag of the key required to unlock the drawer.
     * @param interaction Whether the drawer is interactable.
     */
    Drawer(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
           std::string keyTag = "", bool interaction = true);

    /**
     * @brief Handles player interaction with the drawer.
     *
     * Behavior:
     * - If locked: checks for correct key, otherwise plays locked animation
     * - If unlocked: toggles open/close state
     * - Plays appropriate sound effects
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the drawer each frame.
     *
     * Handles:
     * - Locked drawer shaking animation
     * - Smooth movement animation for opening/closing
     * - Updating collision box and contained entity position
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Sets whether the drawer is locked.
     *
     * @param state True to lock the drawer, false to unlock it.
     */
    void setLocked(bool state) { isLocked = state; }

    /**
     * @brief Places an entity inside the drawer.
     *
     * Automatically positions the entity inside the drawer and updates its collision box.
     *
     * @param entity Pointer to the entity to place inside.
     */
    void setContainedEntity(Entity* entity);

    /**
     * @brief Gets the action text displayed when targeting the drawer.
     *
     * @return "Open" if closed or locked, "Close" if open.
     */
    std::string GetActionText() override;

    /**
     * @brief Drawer destructor.
     */
    ~Drawer();

private:

    /** @brief Whether the drawer is currently locked. */
    bool isLocked = true;

    /** @brief Whether the drawer is currently open. */
    bool isMoved = false;

    /** @brief Whether the drawer is currently animating movement. */
    bool isMoving = false;

    /** @brief Whether the drawer is performing an animation. */
    bool isAnimating = false;

    /** @brief Whether the drawer is performing a locked shake animation. */
    bool isTryingToOpen = false;

    /** @brief Whether the player has attempted to open the locked drawer. */
    bool triedToOpen = false;

    /** @brief Counter used for locked drawer shaking animation. */
    float animCounter = 0.0f;

    /** @brief Original resting position of the drawer. */
    glm::vec3 originalPosition;

    /** @brief Target position when the drawer is opened. */
    glm::vec3 movedPosition;

    /** @brief Tag of the key required to unlock the drawer. */
    std::string drawerKeyTag;

    /** @brief Optional entity contained inside the drawer. */
    Entity* containedEntity = nullptr;

    /** @brief Path to the drawer opening sound effect. */
    std::string drawerOpeningSoundPath = "Audio/drawer_opening.mp3";

    /** @brief Path to the drawer closing sound effect. */
    std::string drawerClosingSoundPath = "Audio/drawer_closing.mp3";

    /** @brief Path to the locked drawer sound effect. */
    std::string drawerLockedSoundPath = "Audio/drawer_locked.mp3";
};
