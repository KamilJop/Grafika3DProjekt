#pragma once
#include "Entity.h"
#include "../Core/Inventory.h"
#include "../Core/Scene.h"
#include "../Light/PointLight.h"

/**
 * @class Pedestal
 * @brief Represents a pedestal that can accept and display specific items.
 *
 * A Pedestal:
 * - Allows the player to place or remove items
 * - Displays the placed item as a rotating 3D model
 * - Emits colored light depending on whether the correct item is placed
 * - Can be used for puzzles requiring item placement validation
 */
class Pedestal : public Entity
{
public:

    /**
     * @brief Constructs a new Pedestal entity.
     *
     * Initializes:
     * - Base Entity properties
     * - Scene reference for spawning placed item models
     * - Light source positioned above the pedestal
     * - Light initially disabled
     *
     * @param model Pointer to the pedestal model.
     * @param pos World position.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param scene Pointer to the scene for spawning placed items.
     * @param pl Pointer to the PointLight associated with the pedestal.
     * @param interaction Whether the pedestal is interactable.
     */
    Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
             Scene* scene, PointLight* pl, bool interaction = false);

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
    void Interact(Inventory* playerInventory) override;

    /**
     * @brief Updates the pedestal each frame.
     *
     * Behavior:
     * - Emits green light when the correct item is placed
     * - Emits red light otherwise
     * - Rotates the placed item model
     * - Checks whether the placed item matches the required item tag
     *
     * @param deltaTime Time elapsed since last frame.
     */
    void Update(float deltaTime) override;

    /**
     * @brief Gets the action text displayed when targeting the pedestal.
     *
     * @return "Examine"
     */
    std::string GetActionText() override { return "Examine"; }

    /**
     * @brief Returns whether the correct item is currently placed.
     *
     * @return True if the correct item is placed, false otherwise.
     */
    bool getHasCorrectItem() { return hasCorrectItem; }

    /**
     * @brief Sets the tag of the item required for the pedestal to be considered correct.
     *
     * @param tag The required item tag.
     */
    void setCorrectItemTag(const std::string& tag) { correctItemTag = tag; }

private:

    /** @brief The 3D entity representing the placed item. */
    Entity* pedestalEntity;

    /** @brief Stored item data for returning it to the inventory. */
    Item* storedItem;

    /** @brief Reference to the scene for spawning item models. */
    Scene* currentScene;

    /** @brief Light source that changes color based on correctness. */
    PointLight* pedestalLight;

    /** @brief Whether an item is currently placed on the pedestal. */
    bool isItemPlaced = false;

    /** @brief Whether the placed item matches the required tag. */
    bool hasCorrectItem = false;

    /** @brief The tag of the item required for puzzle correctness. */
    std::string correctItemTag = "";
};
