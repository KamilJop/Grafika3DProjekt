#pragma once

#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include <vector>
#include "../Core/Scene.h"
#include "Chest.h"

/**
 * @class Lock
 * @brief Represents a rotating‑wheel combination lock puzzle.
 *
 * A Lock consists of:
 * - Several rotating number rolls (each an Entity)
 * - A metal frame piece
 * - A correct numeric combination
 * - A reference to a Chest that unlocks when solved
 *
 * The player can:
 * - Select different rolls
 * - Rotate the selected roll forward or backward
 * - Solve the puzzle by matching the correct combination
 *
 * When solved:
 * - Plays an unlock sound
 * - Unlocks the linked chest (if assigned)
 * - Marks itself for cleanup when appropriate
 */
class Lock : public Entity
{
public:

    /**
     * @brief Constructs a new Lock puzzle entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Creates each roll with positional offsets
     * - Creates the metal frame
     * - Adds all components to the scene
     * - Prepares camera offsets for lock‑picking view
     * - Loads sound effects
     *
     * @param model Base model of the lock.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param models List of models used for each roll.
     * @param metalPart Model used for the metal frame.
     * @param scene Pointer to the scene so rolls and frame can be added.
     * @param interaction Whether the lock itself is interactable.
     */
    Lock(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
         std::vector<Model*> models, Model* metalPart,
         Scene* scene, bool interaction = true);

    /**
     * @brief Locks do not use direct interaction; puzzle interaction is handled externally.
     */
    void Interact(Inventory* playerInventory) override {};

    /** @brief Gets the camera position used when zooming into the lock puzzle. */
    glm::vec3 getLockPuzzlePos() { return lockPuzzlePos; }

    /** @brief Gets the yaw rotation used for the lock puzzle camera. */
    float getLockPuzzleYaw() { return lockPuzzleYaw; }

    /** @brief Gets the pitch rotation used for the lock puzzle camera. */
    float getLockPuzzlePitch() { return lockPuzzlePitch; }

    /** @brief Gets the index of the currently selected roll. */
    int getSelectedIndex() { return selectedIndex; }

    /**
     * @brief Assigns a chest that will be unlocked when the puzzle is solved.
     *
     * @param chest Pointer to the chest to unlock.
     */
    void setChestToUnlock(Chest* chest) { chestToUnlock = chest; }

    /**
     * @brief Sets the correct numeric combination for the lock.
     *
     * @param password Vector of integers representing the correct answer.
     */
    void setLockPassword(std::vector<int> password) { correctAnswer = password; }

    /** @brief Returns whether the lock has been successfully unlocked. */
    bool getIsUnlocked() { return isUnlocked; }

    /** @brief Returns all roll entities for rendering or selection. */
    std::vector<Entity*> getLockRolls() { return LockRolls; }

    /**
     * @brief Gets the action text displayed when targeting the lock.
     *
     * @return "Use"
     */
    std::string GetActionText() override { return "Use"; }

    /**
     * @brief Changes which roll is currently selected.
     *
     * @param direction +1 to move right, -1 to move left.
     */
    void changeSelectedIndex(int direction);

    /**
     * @brief Rotates the currently selected roll.
     *
     * Updates the current combination and checks if the puzzle is solved.
     *
     * @param direction +1 to rotate forward, -1 to rotate backward.
     */
    void moveLockRolls(int direction);

    /**
     * @brief Removes all lock components from the scene.
     *
     * Marks:
     * - All rolls for destruction
     * - The metal frame for destruction
     * - The lock entity itself for destruction
     */
    void cleanUpLock();

    /**
     * @brief Lock destructor.
     */
    ~Lock();

private:

    /** @brief List of rotating number rolls. */
    std::vector<Entity*> LockRolls;

    /** @brief Correct numeric combination. */
    std::vector<int> correctAnswer = { 5, 4, 3, 2 };

    /** @brief Current combination entered by the player. */
    std::vector<int> currentAnswer = { 7, 7, 4, 2 };

    /** @brief Chest that will be unlocked when the puzzle is solved. */
    Chest* chestToUnlock = nullptr;

    /** @brief Index of the currently selected roll. */
    int selectedIndex = 0;

    /** @brief Metal frame model of the lock. */
    Entity* metalPartModel;

    /** @brief Whether the lock has been successfully unlocked. */
    bool isUnlocked = false;

    /** @brief Camera position used when zooming into the lock puzzle. */
    glm::vec3 lockPuzzlePos;

    /** @brief Camera yaw offset for the lock puzzle. */
    float lockPuzzleYaw;

    /** @brief Camera pitch offset for the lock puzzle. */
    float lockPuzzlePitch;

    /** @brief Sound played when rotating a roll. */
    std::string rollSound = "Audio/padlock_move.mp3";

    /** @brief Sound played when the lock is successfully unlocked. */
    std::string unlockSound = "Audio/padlock_unlock.mp3";
};
