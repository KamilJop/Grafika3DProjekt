#include "Lock.h"
#include "Entity.h"

/**
 * @brief Constructs a new Lock puzzle entity.
 *
 * A Lock consists of:
 * - Several rotating number rolls (each represented as an Entity)
 * - A metal frame piece
 * - A correct combination that unlocks the lock
 *
 * This constructor:
 * - Creates each roll with a positional offset
 * - Adds all rolls and the metal part to the scene
 * - Prepares camera positioning for the lock‑picking view
 * - Loads sound effects for rolling and unlocking
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
Lock::Lock(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
           std::vector<Model*> models, Model* metalPart,
           Scene* scene, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    int index = 0;
    glm::vec3 posOffset(0.0f);

    // Create lock rolls with positional offsets
    for (auto* m : models)
    {
        switch (index)
        {
            case 0: posOffset = glm::vec3(0.0375f / 2.0f, 0.11725f / 2.0f, 0.0f); break;
            case 1: posOffset = glm::vec3(0.0125f / 2.0f, 0.11725f / 2.0f, 0.0f); break;
            case 2: posOffset = glm::vec3(-0.013f / 2.0f, 0.11725f / 2.0f, 0.0f); break;
            case 3: posOffset = glm::vec3(-0.038f / 2.0f, 0.11725f / 2.0f, 0.0f); break;
        }
        index++;

        LockRolls.push_back(new Entity(m, pos + posOffset, rot, scal, false));
        scene->AddEntity(LockRolls.back());
    }

    // Create metal frame
    metalPartModel = new Entity(metalPart, pos, rot, scal, false);
    scene->AddEntity(metalPartModel);

    // Setup lock puzzle camera offsets
    lockPuzzleYaw = 90.0f;
    lockPuzzlePitch = 0.0f;
    lockPuzzlePos = pos + glm::vec3(0.0f, 0.1f, -0.25f);

    if (rot.y == 180.0f)
    {
        lockPuzzlePos = pos + glm::vec3(0.0f, 0.1f, 0.25f);
        lockPuzzleYaw *= -1;
    }

    // Load sound effects
    AudioManager::GetInstance().Load3DSoundEffect(rollSound, rollSound);
    AudioManager::GetInstance().Load3DSoundEffect(unlockSound, unlockSound);
}

/**
 * @brief Changes which roll is currently selected.
 *
 * Rolls wrap around when moving past the first or last index.
 *
 * @param direction +1 to move right, -1 to move left.
 */
void Lock::changeSelectedIndex(int direction)
{
    if (isUnlocked) return;

    LockRolls[selectedIndex]->setSelected(false);

    // Reverse direction if lock is rotated
    if (rotation.y == 180.0f)
        direction *= -1;

    selectedIndex += direction;

    if (selectedIndex < 0)
        selectedIndex = LockRolls.size() - 1;

    if (selectedIndex >= LockRolls.size())
        selectedIndex = 0;

    LockRolls[selectedIndex]->setSelected(true);
}

/**
 * @brief Rotates the currently selected roll.
 *
 * Behavior:
 * - Rotates the roll visually
 * - Updates the current combination value
 * - Plays rolling sound
 * - Checks if the combination matches the correct answer
 * - If correct: unlocks the lock and triggers chest opening
 *
 * @param direction +1 to rotate forward, -1 to rotate backward.
 */
void Lock::moveLockRolls(int direction)
{
    if (isUnlocked) return;

    float rotationSpeed = 360.0f / 7.0f * direction;

    // Rotate roll
    LockRolls[selectedIndex]->setRotation(
        LockRolls[selectedIndex]->getRotation() + glm::vec3(rotationSpeed, 0.0f, 0.0f)
    );

    // Update combination number
    currentAnswer[selectedIndex] += direction;

    if (currentAnswer[selectedIndex] >= 8)
        currentAnswer[selectedIndex] = 1;

    if (currentAnswer[selectedIndex] <= 0)
        currentAnswer[selectedIndex] = 7;

    AudioManager::GetInstance().Play3DSoundEffect(rollSound, getPosition(), 0.5f);

    // Check if combination is correct
    bool correct = true;
    for (size_t i = 0; i < correctAnswer.size(); i++)
    {
        if (correctAnswer[i] != currentAnswer[i])
        {
            correct = false;
            break;
        }
    }

    if (correct)
    {
        isUnlocked = true;
        AudioManager::GetInstance().Play3DSoundEffect(unlockSound, getPosition(), 1.0f);

        if (chestToUnlock != nullptr)
            chestToUnlock->OpenChest();
    }
}

/**
 * @brief Removes all lock components from the scene.
 *
 * Marks:
 * - All rolls for destruction
 * - The metal frame for destruction
 * - The lock entity itself for destruction
 */
void Lock::cleanUpLock()
{
    for (auto* roll : LockRolls)
        roll->setShouldGetDestroyed(true);

    metalPartModel->setShouldGetDestroyed(true);
    shouldGetDestroyed = true;
}

/**
 * @brief Lock destructor.
 *
 * Clears pointers to rolls and metal frame.
 */
Lock::~Lock()
{
    for (auto* roll : LockRolls)
        roll = nullptr;

    metalPartModel = nullptr;
}
