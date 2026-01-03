#include "Pedestal.h"

/**
 * @brief Constructs a new Pedestal entity.
 *
 * A Pedestal is an interactable object that:
 * - Accepts specific items placed on top of it
 * - Displays the placed item as a rotating 3D model
 * - Emits colored light depending on whether the correct item is placed
 *
 * This constructor:
 * - Initializes internal pointers
 * - Stores the scene reference
 * - Configures the pedestal's light source (initially off)
 *
 * @param model Pointer to the pedestal model.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param scene Pointer to the scene for spawning placed items.
 * @param pl Pointer to the PointLight associated with the pedestal.
 * @param interaction Whether the pedestal is interactable.
 */
Pedestal::Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                   Scene* scene, PointLight* pl, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    pedestalEntity = nullptr;
    storedItem = nullptr;
    currentScene = scene;
    pedestalLight = pl;

    pedestalLight->setPosition(pos + glm::vec3(0.0f, 2.0f, 0.0f));
    pedestalLight->setAmbientIntensity(0.0f);
    pedestalLight->setDiffuseIntensity(0.0f);
    pedestalLight->setColor(glm::vec3(0.0f));
}

/**
 * @brief Handles interaction with the pedestal.
 *
 * Behavior:
 * - If an item is already placed:
 *   - Returns the item to the player's inventory
 *   - Removes the displayed 3D model
 *   - Resets pedestal state
 *
 * - If no item is placed:
 *   - Checks if the player is holding a valid pedestal item
 *   - Spawns a 3D model of the item on top of the pedestal
 *   - Stores item data internally
 *   - Removes the item from the player's inventory
 *
 * @param playerInventory Pointer to the player's inventory.
 */
void Pedestal::Interact(Inventory* playerInventory)
{
    // Remove placed item
    if (isItemPlaced)
    {
        isItemPlaced = false;

        playerInventory->AddItem(
            storedItem->tag,
            storedItem->title,
            storedItem->imageTexture,
            storedItem->itemModel,
            storedItem->itemScale
        );

        pedestalEntity->setShouldGetDestroyed(true);
        pedestalEntity = nullptr;
        storedItem = nullptr;
        hasCorrectItem = false;
        return;
    }

    // No item in hand
    if (playerInventory->GetCurrentItem() == nullptr)
        return;

    Item* currentItem = playerInventory->GetCurrentItem();

    // Only items with tags starting with "pedestal" are valid
    if (currentItem->tag.length() <= 7)
        return;

    if (currentItem->tag.substr(0, 8) == "pedestal")
    {
        Model* itemModel = currentItem->itemModel;
        glm::vec3 itemScale = currentItem->itemScale;

        // Spawn item model above pedestal
        pedestalEntity = new Entity(
            itemModel,
            getPosition() + glm::vec3(0.0f, 1.3f, 0.0f),
            rotation,
            itemScale,
            false
        );

        currentScene->AddEntity(pedestalEntity);

        // Store item data
        storedItem = new Item{
            currentItem->tag,
            currentItem->title,
            currentItem->imageTexture,
            currentItem->itemModel,
            currentItem->itemScale
        };

        playerInventory->RemoveItem(storedItem->tag);
        isItemPlaced = true;
    }
}

/**
 * @brief Updates the pedestal each frame.
 *
 * Behavior:
 * - If visible:
 *   - Emits green light when the correct item is placed
 *   - Emits red light otherwise
 *
 * - If an item is placed:
 *   - Rotates the displayed 3D model
 *   - Checks whether the placed item matches the required item tag
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Pedestal::Update(float deltaTime)
{
    // Update pedestal light color
    if (isVisible)
    {
        if (hasCorrectItem)
        {
            pedestalLight->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
            pedestalLight->setDiffuseIntensity(1.0f);
            pedestalLight->setAmbientIntensity(0.5f);
        }
        else
        {
            pedestalLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
            pedestalLight->setDiffuseIntensity(1.0f);
            pedestalLight->setAmbientIntensity(0.5f);
        }
    }

    // Rotate placed item and check correctness
    if (pedestalEntity != nullptr)
    {
        pedestalEntity->setRotation(
            pedestalEntity->getRotation() + glm::vec3(0.0f, 30.0f * deltaTime, 0.0f)
        );

        hasCorrectItem = (storedItem->tag == correctItemTag);
    }
}
