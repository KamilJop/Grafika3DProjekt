#include "HauntedEntity.h"

/**
 * @brief Constructs a new HauntedEntity.
 *
 * A HauntedEntity is an object that visually transforms into an alternate model
 * once a specific puzzle condition is met (in this case, the CandlePuzzle).
 *
 * Initializes:
 * - Base Entity properties
 * - Stores the alternate model used after transformation
 *
 * @param model The default model shown before the puzzle is solved.
 * @param secondModel The alternate model shown after the puzzle is solved.
 * @param pos World position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param interaction Whether the entity is interactable.
 */
HauntedEntity::HauntedEntity(Model* model, Model* secondModel,
                             glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
                             bool interaction)
    : Entity(model, pos, rot, scal, interaction), alternateModel(secondModel)
{
    alternateModel = secondModel;
}

/**
 * @brief Updates the haunted entity each frame.
 *
 * Behavior:
 * - Resets examination state every frame
 * - If the entity has already switched models, do nothing
 * - If the CandlePuzzle is solved, replace the current model with the alternate one
 *
 * @param deltaTime Time elapsed since last frame.
 */
void HauntedEntity::Update(float deltaTime)
{
    hasExamined = false;

    // Prevent switching more than once
    if (hasSwitched)
        return;

    // Switch model when puzzle is solved
    if (CandlePuzzle::isSolved)
    {
        entityModel = alternateModel;
        hasSwitched = true;
    }
}
