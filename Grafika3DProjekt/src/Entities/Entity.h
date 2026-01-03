#pragma once
#define NOMINMAX
#include <Windows.h>
#include "../Core/Config.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include "Rendering/Shader.h"
#include "Rendering/CollisionBox.h"
#include "Rendering/Model.h"
#include "Rendering/Material.h"
#include "../Core/Inventory.h"
#include "../UI/UI.h"

/**
 * @class Entity
 * @brief Base class for all world objects in the game.
 *
 * The Entity class provides:
 * - Transform data (position, rotation, scale)
 * - Rendering support (model, shader, model matrix)
 * - Collision box calculation
 * - Interaction system (interactable, pickable, action text)
 * - Visibility, outline, and shadow toggles
 *
 * All interactable objects (doors, drawers, items, puzzles, etc.)
 * inherit from this class.
 */
class Entity
{
public:

    /**
     * @brief Constructs a new Entity.
     *
     * Initializes:
     * - Transform values
     * - Model reference
     * - Collision box (from model if available)
     * - Interaction flag
     * - Model matrix and world‑space collision box
     *
     * @param model Pointer to the 3D model.
     * @param pos Initial world position.
     * @param rot Initial rotation vector.
     * @param scal Initial scale vector.
     * @param interaction Whether the entity is interactable.
     */
    Entity(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction = false);

    /**
     * @brief Entity destructor.
     */
    ~Entity();

    /**
     * @brief Gets the current model matrix.
     *
     * Recalculates the matrix each call to ensure it reflects
     * the latest transform values.
     *
     * @return The updated model matrix.
     */
    glm::mat4 GetModelMatrix();

    /** @brief Sets the entity's world position. */
    void setPosition(glm::vec3 newPosition);

    /** @brief Sets the entity's rotation. */
    void setRotation(glm::vec3 newRotation);

    /** @brief Sets the entity's scale. */
    void setScale(glm::vec3 newScale);

    /** @brief Enables or disables shadow casting. */
    void setCastsShadow(bool state) { castsShadow = state; }

    /** @brief Sets the display title of the entity. */
    void setTitle(std::string newTitle) { title = newTitle; }

    /** @brief Enables or disables collision detection. */
    void setColissions(bool state) { hasCollisions = state; }

    /** @brief Sets a new model and updates collision box. */
    void setModel(Model* model) { entityModel = model; UpdateCollisionBox(); }

    /** @brief Marks the entity for destruction. */
    void setShouldGetDestroyed(bool state) { shouldGetDestroyed = state; }

    /** @brief Enables or disables interaction. */
    void setInteractable(bool state) { interactable = state; }

    /** @brief Gets the entity's world position. */
    glm::vec3 getPosition();

    /** @brief Gets the entity's rotation. */
    glm::vec3 getRotation();

    /** @brief Gets the entity's scale. */
    glm::vec3 getScale();

    /** @brief Returns whether the entity is interactable. */
    bool getInteractable() { return interactable; }

    /** @brief Returns whether the entity casts shadows. */
    bool getCastsShadow() { return castsShadow; }

    /** @brief Returns whether the entity has collisions enabled. */
    bool getColissions() { return hasCollisions; }

    /** @brief Returns whether the entity can be picked up. */
    bool getPickable() { return pickable; }

    /** @brief Returns whether the entity should be destroyed. */
    bool getShouldGetDestroyed() { return shouldGetDestroyed; }

    /**
     * @brief Gets the entity's texture (default: nullptr).
     *
     * Derived classes may override this.
     */
    virtual Texture* getTexture() { return nullptr; }

    /** @brief Gets the entity's tag (used for keys, items, etc.). */
    std::string getTag() { return itemTag; }

    /** @brief Gets the display title of the entity. */
    std::string getTitle() { return title; }

    /** @brief Gets the model used by this entity. */
    Model* getModel() { return entityModel; }

    /** @brief Returns whether the entity is outlined. */
    bool isOutlined() { return outlined; }

    /** @brief Sets whether the entity is selected. */
    void setSelected(bool state) { selected = state; }

    /** @brief Returns whether the entity is selected. */
    bool isSelected() { return selected; }

    /** @brief Sets whether the entity is outlined. */
    void setOutlined(bool state) { outlined = state; }

    /** @brief Sets whether the entity is visible. */
    void setVisibility(bool state) { isVisible = state; }

    /** @brief Returns whether the entity is visible. */
    bool getVisible() { return isVisible; }

    /** @brief Gets the world‑space collision box. */
    CollisionBox GetCollisions() { return collisions; }

    /**
     * @brief Draws the entity using the provided shader.
     *
     * Sends the model matrix to the shader and renders the model.
     *
     * @param shader Pointer to the shader used for rendering.
     */
    void DrawEntity(Shader* shader);

    /**
     * @brief Updates the world‑space collision box.
     *
     * Transforms all 8 corners of the model's local AABB using the model matrix,
     * then computes the new world‑space min/max bounds.
     */
    void UpdateCollisionBox();

    /** @brief Sets the action text displayed when interacting. */
    void setActionText(std::string text) { actionText = text; }

    /** @brief Sets the examine text displayed when inspecting the entity. */
    void setExamineText(std::string text) { examineText = text; }

    /** @brief Sets the entity's tag. */
    void setTag(std::string tag) { itemTag = tag; }

    /**
     * @brief Updates the entity each frame.
     *
     * Default implementation does nothing.
     *
     * @param deltaTime Time elapsed since last frame.
     */
    virtual void Update(float deltaTime) {};

    /**
     * @brief Gets the action text displayed when targeting the entity.
     *
     * @return The current action text (default: "Examine").
     */
    virtual std::string GetActionText() { return actionText; }

    /**
     * @brief Handles player interaction with the entity.
     *
     * Default behavior: displays the examine text as a subtitle.
     *
     * @param playerInventory Pointer to the player's inventory.
     */
    virtual void Interact(Inventory* playerInventory);

protected:

    /** @brief Pointer to the entity's 3D model. */
    Model* entityModel;

    /** @brief World position of the entity. */
    glm::vec3 position;

    /** @brief Rotation vector (degrees). */
    glm::vec3 rotation;

    /** @brief Scale vector. */
    glm::vec3 scale;

    /** @brief Whether the entity has been examined before. */
    bool hasExamined = false;

    /** @brief Text shown when examining the entity. */
    std::string examineText = "You see nothing special.";

    /** @brief Whether the entity casts shadows. */
    bool castsShadow = true;

    /** @brief Whether the entity is outlined (highlighted). */
    bool outlined = false;

    /** @brief Whether the entity has collision detection enabled. */
    bool hasCollisions = true;

    /** @brief Whether the entity can be interacted with. */
    bool interactable;

    /** @brief Whether the entity should be destroyed. */
    bool shouldGetDestroyed = false;

    /** @brief Whether the entity can be picked up. */
    bool pickable = false;

    /** @brief Whether the entity is visible. */
    bool isVisible = true;

    /** @brief Tag used for identifying items (keys, puzzle objects, etc.). */
    std::string itemTag = "";

    /** @brief World‑space collision box. */
    CollisionBox collisions;

    /** @brief Display title of the entity. */
    std::string title = "Untitled object";

    /** @brief Whether the entity is currently selected. */
    bool selected = false;

    /** @brief Cached model matrix. */
    glm::mat4 modelMatrix;

    /**
     * @brief Calculates the model matrix from position, rotation, and scale.
     *
     * @return The calculated model matrix.
     */
    glm::mat4 CalculateModelMatrix();

    /** @brief Text displayed when interacting (default: "Examine"). */
    std::string actionText = "Examine";
};
