#include "Chest.h"

/**
 * @brief Constructs a new Chest entity composed of a lower and upper part.
 *
 * Initializes:
 * - Base Entity (lower chest body)
 * - Creates the upper chest lid as a separate Entity
 * - Adjusts lid offset depending on rotation
 * - Registers the lid entity inside the scene
 * - Preloads chest opening sound
 *
 * @param lowerPart Pointer to the model for the lower chest body.
 * @param upperPartModel Pointer to the model for the chest lid.
 * @param pos World position of the chest.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param scene Pointer to the scene so the lid can be added as an entity.
 * @param interaction Whether the chest is interactable.
 */
Chest::Chest(Model* lowerPart, Model* upperPartModel, glm::vec3 pos, glm::vec3 rot,
             glm::vec3 scal, Scene* scene, bool interaction)
    : Entity(lowerPart, pos, rot, scal, interaction)
{
    // Offset for the lid depending on chest orientation
    glm::vec3 upperOffset = glm::vec3(0.0f, 0.55f, 0.45f);
    if (rot.y == 180.0f)
    {
        upperOffset = glm::vec3(0.0f, 0.55f, -0.45f);
    }

    // Create the upper lid entity
    upperPart = new Entity(upperPartModel, pos + upperOffset, rot, scal, false);
    scene->AddEntity(upperPart);

    // Target rotation for opening animation
    targetRotation = rot.x + 70.0f;

    // Load chest opening sound
    AudioManager::GetInstance().Load3DSoundEffect(chestOpenSound, chestOpenSound);
}

/**
 * @brief Handles player interaction with the chest.
 *
 * Behavior:
 * - If locked: show subtitle once and prevent opening
 * - If unlocked: play opening sound and begin lid animation
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Chest::Interact(Inventory* playerInventory)
{
    if (isLocked)
    {
        if (!triedToOpen)
        {
            triedToOpen = true;
            UI::SetSubtitle("The chest is locked. I need to find password to padlock.", 2.0f);
        }
        return;
    }

    isOpen = true;
    AudioManager::GetInstance().Play3DSoundEffect(chestOpenSound, getPosition());
}

/**
 * @brief Updates the chest each frame.
 *
 * Handles:
 * - Lid opening animation (rotating the upper part)
 * - Disables interaction and outline once opening begins
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Chest::Update(float deltaTime)
{
    if (!isOpen)
        return;

    // Animate lid opening
    if (upperPart->getRotation().x < targetRotation)
    {
        float rotationSpeed = 17.5f;
        float newRotationX = upperPart->getRotation().x + rotationSpeed * deltaTime;

        if (newRotationX > targetRotation)
        {
            newRotationX = targetRotation;
        }

        upperPart->setRotation(glm::vec3(
            newRotationX,
            upperPart->getRotation().y,
            upperPart->getRotation().z
        ));

        // Disable interaction and outline once opening starts
        interactable = false;
        outlined = false;
    }
}
