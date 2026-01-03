#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include "../Core/Config.h"
#include <string>
#include <math.h>

/**
 * @class Door
 * @brief Represents an interactable door that can be locked, unlocked, opened, and closed.
 *
 * The Door class provides:
 * - Lock/unlock logic using key tags
 * - Smooth opening/closing animation
 * - Shaking animation when trying to open a locked door
 * - Sound effects for all door states
 * - Optional door frame entity for enabling/disabling collisions
 */
class Door : public Entity
{
public:

    /**
     * @brief Constructs a new Door entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Door state flags (open, locked, animating)
     * - Door title and rotation baseline
     * - Optional door frame reference (for collision toggling)
     * - Key tag required to unlock the door
     * - Loads all door-related sound effects
     *
     * @param model Pointer to the door model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param name Display name of the door.
     * @param frame Pointer to the door frame entity (may be nullptr).
     * @param keyTag Tag of the key required to unlock the door.
     * @param interaction Whether the door is interactable.
     */
    Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
         std::string name, Entity* frame,
         std::string keyTag = "No needed", bool interaction = true);

    /**
     * @brief Door destructor.
     */
    ~Door();

    /**
     * @brief Sets whether the door is locked.
     *
     * @param state True to lock the door, false to unlock it.
     */
    void setLocked(bool state) { isLocked = state; }

    /**
     * @brief Sets whether the door is open.
     *
     * @param state True to mark the door as open.
     */
    void setIsOpen(bool state) { isOpen = state; }

    /**
     * @brief Handles player interaction with the door.
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
     * @brief Sets whether the player has attempted to open the locked door.
     *
     * @param state True if the player tried to open the locked door.
     */
    void setTriedToOpen(bool state) { triedToOpen = state; }

    /**
     * @brief Sets the subtitle text shown when examining a locked door.
     *
     * @param text New examine text.
     */
    void setExamineText(const std::string& text) { examineText = text; }

    /**
     * @brief Updates the door each frame.
     *
     * Handles:
     * - Locked door shaking animation
     * - Smooth rotation animation for opening/closing
     * - Collision box updates
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Sets the direction of the door's opening rotation.
     *
     * @param dir 1 for clockwise, -1 for counterclockwise.
     */
    void setDirection(int dir) { direction = dir; }

    /**
     * @brief Gets the action text displayed when targeting the door.
     *
     * @return "Close Door" if open, otherwise "Open Door".
     */
    std::string GetActionText() override;

private:

    /** @brief Whether the door is currently open. */
    bool isOpen;

    /** @brief Whether the door is currently animating. */
    bool isAnimating;

    /** @brief Whether the door is performing a locked shake animation. */
    bool isTryingToOpen;

    /** @brief Whether the player has attempted to open the locked door. */
    bool triedToOpen = false;

    /** @brief Starting Y rotation used as the baseline for animations. */
    float startRotationY = 0.0f;

    /** @brief Whether the door is locked. */
    bool isLocked = false;

    /** @brief Direction of door rotation (1 or -1). */
    int direction = 1;

    /** @brief Tag of the key required to unlock the door. */
    std::string doorKeyTag;

    /** @brief Pointer to the door frame entity (may be nullptr). */
    Entity* doorFrame;

    /** @brief Global configuration reference. */
    Config& config = Config::getInstance();

    /** @brief Path to the door opening sound effect. */
    std::string openingSoundPath = "Audio/door_opening.mp3";

    /** @brief Path to the door closing sound effect. */
    std::string closingSoundPath = "Audio/door_closing.mp3";

    /** @brief Path to the locked door sound effect. */
    std::string lockedSoundPath = "Audio/door_locked.mp3";

    /** @brief Path to the unlocking sound effect. */
    std::string unlockingSoundPath = "Audio/door_unlocking.mp3";

    /** @brief Sound effect name used for opening. */
    std::string openingSoundName = "door_opening";

    /** @brief Sound effect name used for closing. */
    std::string closingSoundName = "door_closing";

    /** @brief Sound effect name used for locked door attempts. */
    std::string lockedSoundName = "door_locked";

    /** @brief Subtitle text shown when examining a locked door. */
    std::string examineText = "The door is locked.I probably need to find a key.";

    /** @brief Sound effect name used for unlocking. */
    std::string unlockingSoundName = "door_unlocking";

    /** @brief Counter used for locked door shaking animation. */
    float animCounter = 0.0f;
};
