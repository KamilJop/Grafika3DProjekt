#include "Desk.h"

/**
 * @brief Constructs a Desk entity composed of multiple drawers and a desk door.
 *
 * Initializes:
 * - Base desk model (this Entity)
 * - Creates drawer entities from the provided drawer models
 * - Assigns each drawer a unique key tag (drawerKey1, drawerKey2, ...)
 * - Adds drawers to the scene
 * - Unlocks the third drawer by default
 * - Creates and positions the desk door entity
 * - Adds the desk door to the scene
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
Desk::Desk(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
           std::vector<Model*> drawerModels, Model* doorModel,
           Scene* scene, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    int index = 0;

    // Create drawers
    for (auto* m : drawerModels)
    {
        std::string drawerKeyTag = "drawerKey" + std::to_string(index + 1);

        drawers.push_back(new Drawer(m, pos, rot, scal, drawerKeyTag, true));
        scene->AddEntity(drawers.back());

        drawers.back()->setTitle("Drawer");

        // Unlock the third drawer by default
        if (index == 2)
            drawers.back()->setLocked(false);

        index++;
    }

    // Create desk door
    deskDoor = new Door(
        doorModel,
        pos + glm::vec3(-1.02f, 0.6f, 0.32f),
        rot,
        scal,
        "Desk Door",
        nullptr
    );

    deskDoor->setDirection(-1);
    scene->AddEntity(deskDoor);
}

/**
 * @brief Places an entity inside the second drawer.
 *
 * This is used to hide items inside the desk.
 *
 * @param entity Pointer to the entity to place inside the drawer.
 */
void Desk::setDrawerContainedEntity(Entity* entity)
{
    if (drawers.size() > 0)
    {
        drawers[1]->setContainedEntity(entity);
    }
}
