#include "Entity.h"

/**
 * @brief Constructs a new Entity.
 *
 * Initializes:
 * - Position, rotation, and scale
 * - Model reference and model matrix
 * - Collision box (from model if available)
 * - Interaction flag
 * - Updates world‑space collision box
 *
 * @param model Pointer to the 3D model used by this entity.
 * @param pos Initial world position.
 * @param rot Initial rotation vector.
 * @param scal Initial scale vector.
 * @param interaction Whether the entity is interactable.
 */
Entity::Entity(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
{
    position = pos;
    rotation = rot;
    scale = scal;
    entityModel = model;

    modelMatrix = CalculateModelMatrix();

    if (model != nullptr)
    {
        collisions = model->GetCollisionBox();
    }
    else
    {
        collisions.min = glm::vec3(0.0f);
        collisions.max = glm::vec3(0.0f);
    }

    interactable = interaction;

    // Initialize world‑space collision box
    UpdateCollisionBox();
}

/**
 * @brief Entity destructor.
 *
 * Resets transform values and clears model reference.
 */
Entity::~Entity()
{
    position = glm::vec3(0.0f);
    rotation = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    entityModel = nullptr;
}

/**
 * @brief Gets the current model matrix.
 *
 * Recalculates the matrix each call to ensure it reflects
 * the latest position, rotation, and scale.
 *
 * @return The updated model matrix.
 */
glm::mat4 Entity::GetModelMatrix()
{
    modelMatrix = CalculateModelMatrix();
    return modelMatrix;
}

/**
 * @brief Sets the entity's world position.
 *
 * @param newPosition New position vector.
 */
void Entity::setPosition(glm::vec3 newPosition)
{
    position = newPosition;
}

/**
 * @brief Sets the entity's rotation.
 *
 * @param newRotation New rotation vector.
 */
void Entity::setRotation(glm::vec3 newRotation)
{
    rotation = newRotation;
}

/**
 * @brief Sets the entity's scale.
 *
 * @param newScale New scale vector.
 */
void Entity::setScale(glm::vec3 newScale)
{
    scale = newScale;
}

/**
 * @brief Gets the entity's world position.
 *
 * @return Position vector.
 */
glm::vec3 Entity::getPosition()
{
    return position;
}

/**
 * @brief Gets the entity's rotation.
 *
 * @return Rotation vector.
 */
glm::vec3 Entity::getRotation()
{
    return rotation;
}

/**
 * @brief Gets the entity's scale.
 *
 * @return Scale vector.
 */
glm::vec3 Entity::getScale()
{
    return scale;
}

/**
 * @brief Calculates the model matrix from position, rotation, and scale.
 *
 * Order of transformations:
 * 1. Translation  
 * 2. Rotation (Y → X → Z)  
 * 3. Scaling
 *
 * @return The calculated model matrix.
 */
glm::mat4 Entity::CalculateModelMatrix()
{
    glm::mat4 model(1.0f);

    model = glm::translate(model, position);

    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    model = glm::scale(model, scale);

    return model;
}

/**
 * @brief Draws the entity using the provided shader.
 *
 * Sends the model matrix to the shader and renders the model.
 *
 * @param shader Pointer to the shader used for rendering.
 */
void Entity::DrawEntity(Shader* shader)
{
    if (entityModel == nullptr)
        return;

    modelMatrix = CalculateModelMatrix();
    shader->setMat4("model", modelMatrix);

    entityModel->RenderModel(shader);
}

/**
 * @brief Updates the world‑space collision box.
 *
 * Transforms all 8 corners of the model's local AABB using the model matrix,
 * then computes the new world‑space min/max bounds.
 */
void Entity::UpdateCollisionBox()
{
    if (entityModel == nullptr)
        return;

    glm::mat4 modelMatrix = GetModelMatrix();

    glm::vec3 localMin = entityModel->GetCollisionBox().min;
    glm::vec3 localMax = entityModel->GetCollisionBox().max;

    glm::vec3 corners[8] = {
        {localMin.x, localMin.y, localMin.z},
        {localMin.x, localMin.y, localMax.z},
        {localMin.x, localMax.y, localMin.z},
        {localMin.x, localMax.y, localMax.z},
        {localMax.x, localMin.y, localMin.z},
        {localMax.x, localMin.y, localMax.z},
        {localMax.x, localMax.y, localMin.z},
        {localMax.x, localMax.y, localMax.z}
    };

    collisions.min = glm::vec3(10000.0f);
    collisions.max = glm::vec3(-10000.0f);

    for (int i = 0; i < 8; i++)
    {
        glm::vec4 transformed = modelMatrix * glm::vec4(corners[i], 1.0f);

        collisions.min = glm::min(collisions.min, glm::vec3(transformed));
        collisions.max = glm::max(collisions.max, glm::vec3(transformed));
    }
}

/**
 * @brief Default interaction behavior.
 *
 * Displays the entity's examine text as a subtitle.
 *
 * @param playerInventory Pointer to the player's inventory (unused).
 */
void Entity::Interact(Inventory* playerInventory)
{
    UI::SetSubtitle(examineText, 3.0f);
}
