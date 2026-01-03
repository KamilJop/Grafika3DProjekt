#include "Door.h"

/**
 * @brief Constructs a new Door entity.
 *
 * Initializes:
 * - Base Entity properties
 * - Door state flags (open, animating, trying to open)
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
Door::Door(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
           std::string name, Entity* frame, std::string keyTag, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    isOpen = false;
    isAnimating = false;
    isTryingToOpen = false;

    title = name;
    startRotationY = rot.y;

    doorFrame = frame;
    doorKeyTag = keyTag;

    // Load sound effects
    AudioManager::GetInstance().Load3DSoundEffect("door_opening", openingSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect("door_closing", closingSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect("door_locked", lockedSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect("door_unlocking", unlockingSoundPath);
}

/**
 * @brief Door destructor.
 */
Door::~Door()
{
    isOpen = false;
    isAnimating = false;
    isTryingToOpen = false;
    title = "";
    doorFrame = nullptr;
}

/**
 * @brief Handles player interaction with the door.
 *
 * Behavior:
 * - If the door is animating or shaking, ignore interaction
 * - If locked:
 *   - If player holds the correct key: unlock the door
 *   - Otherwise: play locked sound and shake animation
 * - If unlocked:
 *   - Toggle open/close state
 *   - Play appropriate sound
 *   - Enable/disable door frame collisions
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Door::Interact(Inventory* playerInventory)
{
    if (isAnimating || isTryingToOpen)
        return;

    // Locked door logic
    if (isLocked)
    {
        // Player has the correct key
        if (playerInventory->GetCurrentItem()->tag == doorKeyTag)
        {
            playerInventory->RemoveItem(doorKeyTag);
            isLocked = false;

            AudioManager::GetInstance().Play3DSoundEffect(
                unlockingSoundName, position, config.sfxVolume * 2.0f
            );
            return;
        }

        // Player does not have the key
        UI::SetSubtitle(examineText, 3.0f);
        isTryingToOpen = true;
        animCounter = 0.0f;

        AudioManager::GetInstance().Play3DSoundEffect(
            lockedSoundName, position, config.sfxVolume * 2.0f
        );
        return;
    }

    // Toggle open/close
    isOpen = !isOpen;

    if (isOpen)
    {
        if (doorFrame != nullptr)
            doorFrame->setColissions(false);

        AudioManager::GetInstance().Play3DSoundEffect(
            openingSoundName, position, config.sfxVolume * 2.0f
        );
    }
    else
    {
        if (doorFrame != nullptr)
            doorFrame->setColissions(true);

        AudioManager::GetInstance().Play3DSoundEffect(
            closingSoundName, position, config.sfxVolume * 2.0f
        );
    }
}

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
void Door::Update(float deltaTime)
{
    // Locked door shaking animation
    if (isTryingToOpen)
    {
        animCounter += deltaTime;

        if (animCounter < 0.5f)
        {
            float shakeOffset = sin(animCounter * 30.0f) * 2.0f;
            rotation.y = startRotationY + shakeOffset;
        }
        else
        {
            rotation.y = startRotationY;
            isTryingToOpen = false;
            animCounter = 0.0f;
        }
        return;
    }

    // Reset animation flags
    isAnimating = false;
    isTryingToOpen = false;
    animCounter = 0.0f;

    // Determine target rotation
    float targetAngle = isOpen
        ? (startRotationY + (90.0f * direction))
        : startRotationY;

    float currentAngle = rotation.y;

    // Determine if door is still animating
    isAnimating = (abs(currentAngle - targetAngle) > 0.8f);

    // Smooth rotation
    float newAngle = glm::mix(currentAngle, targetAngle, 5.0f * deltaTime);
    rotation.y = newAngle;

    UpdateCollisionBox();
}

/**
 * @brief Gets the action text displayed when targeting the door.
 *
 * @return "Close Door" if open, otherwise "Open Door".
 */
std::string Door::GetActionText()
{
    return isOpen ? "Close Door" : "Open Door";
}
