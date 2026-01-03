#pragma once
#include "Entity.h"
#include "Drawer.h"
#include "Door.h"
#include "../Systems/AudioManager.h"
#include <string>
#include <vector>
#include "../Core/Scene.h"

/**
 * @class Desk
 * @brief Represents a desk composed of multiple drawers and a desk door.
 *
 * The Desk class:
 * - Creates and manages several Drawer entities
 * - Creates a Door entity representing the desk cabinet
 * - Allows placing an item inside a specific drawer
 * - Registers all sub‑entities inside the Scene
 */
class Desk : public Entity
{
public:

    /**
     * @brief Constructs a new Desk entity with drawers and a desk door.
     *
     * Initializes:
     * - Base desk model (this Entity)
     * - Creates drawer entities from the provided drawer models
     * - Assigns each drawer a unique key tag (drawerKey1, drawerKey2, ...)
     * - Unlocks the third drawer by default
     * - Creates and positions the desk door entity
     * - Adds all drawers and the door to the scene
     *
     * @param model Pointer to the desk base model.
     * @param pos World position of the desk.
     * @param rot Rotation vector.
     * @param scal Scale vector.
     * @param drawerModels List of models used to create drawer entities.
     * @param doorModel Model used for the desk door.
     * @param scene Pointer to the scene so drawers and door can be added.
     * @param interaction Whether the desk itself is interactable.
     */
    Desk(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
         std::vector<Model*> drawerModels, Model* doorModel,
         Scene* scene, bool interaction);

    /**
     * @brief Desk destructor.
     */
    ~Desk();

    /**
     * @brief Sets an entity to be contained inside the second drawer.
     *
     * This is used to hide items inside the desk.
     *
     * @param entity Pointer to the entity to place inside the drawer.
     */
    void setDrawerContainedEntity(Entity* entity);

    /**
     * @brief Desk does not have direct interaction behavior.
     *
     * Drawers and the door handle their own interactions.
     */
    void Interact(Inventory* playerInventory) override {};

private:

    /** @brief List of drawer entities belonging to the desk. */
    std::vector<Drawer*> drawers;

    /** @brief Door entity representing the desk cabinet. */
    Door* deskDoor;
};
