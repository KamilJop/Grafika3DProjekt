#include "BookshelfPuzzle.h"

/**
 * @brief Constructs a new BookshelfPuzzle entity.
 *
 * Initializes:
 * - Base Entity properties
 * - Marks the puzzle as non‑pickable
 * - Stores original position
 * - Computes solved position (where the shelf moves when puzzle is solved)
 *
 * @param model Pointer to the 3D model.
 * @param pos Initial world position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param interaction Whether the puzzle object is interactable.
 */
BookshelfPuzzle::BookshelfPuzzle(Model* model, glm::vec3 pos, glm::vec3 rot,
                                 glm::vec3 scal, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    pickable = false;
    originalPosition = pos;
    solvedPosition = pos + glm::vec3(-2.0f, 0.0f, 0.0f);
}

/**
 * @brief Handles player interaction with the bookshelf puzzle.
 *
 * If the puzzle is not already moving, triggers the movement animation.
 *
 * @param playerInventory Pointer to the player's inventory (unused).
 */
void BookshelfPuzzle::Interact(Inventory* playerInventory)
{
    if (isMoving)
        return;

    isMoving = true;
}

/**
 * @brief Updates the bookshelf puzzle each frame.
 *
 * Handles:
 * - Waiting for the book puzzle to be solved
 * - Sliding the bookshelf to its solved position
 * - Updating collision box during movement
 * - Marking the puzzle as finished once movement completes
 *
 * @param deltaTime Time elapsed since last frame.
 */
void BookshelfPuzzle::Update(float deltaTime)
{
    if (ended)
        return;

    // Start moving automatically when the book puzzle is solved
    if (Book::solved && !isMoving)
    {
        isMoving = true;
    }

    if (!isMoving)
        return;

    // Move toward solved position
    position = glm::mix(position, solvedPosition, deltaTime * 0.5f);
    UpdateCollisionBox();

    // Stop when close enough
    if (glm::length(position - solvedPosition) < 0.05f)
    {
        isMoving = false;
        ended = true;
    }
}

/**
 * @brief Destructor for BookshelfPuzzle.
 *
 * Resets movement state.
 */
BookshelfPuzzle::~BookshelfPuzzle()
{
    isMoving = false;
}
