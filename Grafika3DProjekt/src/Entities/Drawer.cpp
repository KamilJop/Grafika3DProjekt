#include "Drawer.h"

/**
 * @brief Constructs a new Drawer entity.
 *
 * Initializes:
 * - Base Entity properties
 * - Stores original position
 * - Assigns key tag required to unlock the drawer
 * - Loads all drawer-related sound effects (open, close, locked)
 *
 * @param model Pointer to the drawer model.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param keyTag Tag of the key required to unlock the drawer.
 * @param interaction Whether the drawer is interactable.
 */
Drawer::Drawer(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
               std::string keyTag, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    originalPosition = position;
    drawerKeyTag = keyTag;

    AudioManager::GetInstance().Load3DSoundEffect(drawerOpeningSoundPath, drawerOpeningSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect(drawerClosingSoundPath, drawerClosingSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect(drawerLockedSoundPath, drawerLockedSoundPath);
}

/**
 * @brief Handles player interaction with the drawer.
 *
 * Behavior:
 * - If locked:
 *   - If player has correct key: unlock drawer
 *   - Otherwise: play locked sound and shake animation
 * - If unlocked:
 *   - Toggle between open and closed states
 *   - Play appropriate sound effect
 *   - Disable interaction if an item is inside the drawer
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Drawer::Interact(Inventory* playerInventory)
{
    if (isMoving || isAnimating)
        return;

    // Locked drawer logic
    if (isLocked)
    {
        if (playerInventory->GetCurrentItem()->tag == drawerKeyTag)
        {
            playerInventory->RemoveItem(drawerKeyTag);
            isLocked = false;

            AudioManager::GetInstance().Play3DSoundEffect(
                drawerLockedSoundPath, position, Config::getInstance().sfxVolume * 2.0f
            );
            return;
        }

        if (!triedToOpen)
        {
            triedToOpen = true;
            UI::SetSubtitle("The drawer is locked. I probably need to find a key.", 3.0f);
        }

        isTryingToOpen = true;
        animCounter = 0.0f;

        AudioManager::GetInstance().Play3DSoundEffect(
            drawerLockedSoundPath, position, Config::getInstance().sfxVolume * 2.0f
        );
        return;
    }

    // Toggle open/close
    if (isMoved)
    {
        movedPosition = originalPosition;

        AudioManager::GetInstance().Play3DSoundEffect(
            drawerClosingSoundPath, position, Config::getInstance().sfxVolume * 2.0f
        );
    }
    else
    {
        movedPosition = originalPosition + glm::vec3(0.0f, 0.0f, 0.3f);

        // If drawer contains an item, disable interaction until item is removed
        if (containedEntity != nullptr)
        {
            interactable = false;
            outlined = false;
        }

        AudioManager::GetInstance().Play3DSoundEffect(
            drawerOpeningSoundPath, position, Config::getInstance().sfxVolume * 2.0f
        );
    }

    isMoving = true;
}

/**
 * @brief Updates the drawer each frame.
 *
 * Handles:
 * - Removing contained entity reference when far away
 * - Locked drawer shaking animation
 * - Smooth movement animation for opening/closing
 * - Updating collision box and contained entity position
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Drawer::Update(float deltaTime)
{
    // Check if contained entity has been removed from drawer
    if (containedEntity != nullptr)
    {
        float distance = glm::length(containedEntity->getPosition() - position);
        if (distance > 1.5f)
        {
            containedEntity = nullptr;
        }
    }

    if (containedEntity == nullptr)
    {
        interactable = true;
    }

    // Locked drawer shake animation
    if (isTryingToOpen)
    {
        animCounter += deltaTime;

        if (animCounter < 0.35f)
        {
            float shakeOffset = sin(animCounter * 15.0f) * 2.0f;
            rotation.x = shakeOffset;
        }
        else
        {
            rotation.x = 0.0f;
            isTryingToOpen = false;
            animCounter = 0.0f;
        }
        return;
    }

    isAnimating = false;
    isTryingToOpen = false;
    animCounter = 0.0f;

    // Drawer movement animation
    if (isMoving)
    {
        float moveSpeed = 3.0f;

        position = glm::mix(position, movedPosition, moveSpeed * deltaTime);

        // Move contained entity along with drawer
        if (containedEntity != nullptr)
        {
            containedEntity->setPosition(position + glm::vec3(0.7f, 0.5f, 0.0f));
            containedEntity->UpdateCollisionBox();
        }

        UpdateCollisionBox();

        // Stop animation when close enough
        if (glm::length(position - movedPosition) < 0.03f)
        {
            position = movedPosition;
            isMoving = false;
            isMoved = !isMoved;
        }
    }
}

/**
 * @brief Gets the action text displayed when targeting the drawer.
 *
 * @return "Open" if closed or locked, "Close" if open.
 */
std::string Drawer::GetActionText()
{
    if (isLocked)
        return "Open";

    return isMoved ? "Close" : "Open";
}

/**
 * @brief Places an entity inside the drawer.
 *
 * Automatically positions the entity inside the drawer and updates its collision box.
 *
 * @param entity Pointer to the entity to place inside.
 */
void Drawer::setContainedEntity(Entity* entity)
{
    containedEntity = entity;
    containedEntity->setPosition(position + glm::vec3(0.7f, 0.5f, 0.0f));
    containedEntity->UpdateCollisionBox();
}
