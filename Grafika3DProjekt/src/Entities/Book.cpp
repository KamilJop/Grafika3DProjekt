#include "Book.h"

/**
 * @brief Static container storing tags of currently moved books.
 */
std::vector<std::string> Book::bookTags = {};

/**
 * @brief Global puzzle state indicating whether the bookshelf puzzle is solved.
 */
bool Book::solved = false;

/**
 * @brief Constructs a new Book entity.
 *
 * Initializes:
 * - Base Entity properties
 * - Book tag and movement states
 * - Original, moved, and solved positions
 * - Loads movement and puzzle‑solved sound effects
 *
 * @param model Pointer to the 3D model.
 * @param pos Initial world position.
 * @param rot Rotation vector.
 * @param scal Scale vector.
 * @param tag Identifier tag for the book.
 * @param interaction Whether the book is interactable.
 */
Book::Book(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,
           std::string tag, bool interaction)
    : Entity(model, pos, rot, scal, interaction)
{
    itemTag = tag;
    pickable = false;

    originalPosition = pos;
    movedPosition = pos + glm::vec3(0.0f, 0.0f, 0.23f);
    solvedPosition = pos + glm::vec3(-2.0f, 0.0f, 0.0f);

    // Load puzzle solved sound for specific books
    if (itemTag == "redBook" || itemTag == "orangeBook" ||
        itemTag == "yellowBook" || itemTag == "purpleBook")
    {
        AudioManager::GetInstance().Load3DSoundEffect("bookshelf_solved", bookshelfSolvedSoundPath);
    }

    // Load movement sounds
    for (int i = 0; i < movingSoundPaths.size(); i++)
    {
        AudioManager::GetInstance().Load3DSoundEffect("book_move_" + std::to_string(i), movingSoundPaths[i]);
    }
}

/**
 * @brief Handles player interaction with the book.
 *
 * Plays a random movement sound and toggles the book between
 * moved and original positions. Updates the global puzzle state list.
 *
 * @param playerInventory Pointer to the player's inventory (unused).
 */
void Book::Interact(Inventory* playerInventory)
{
    int soundIndex = rand() % movingSoundPaths.size();

    if (Book::solved)
        return;

    AudioManager::GetInstance().Play3DSoundEffect("book_move_" + std::to_string(soundIndex), position, 1.0f);

    if (!isMoved)
    {
        isMoving = true;
        isMoved = true;
        bookTags.push_back(itemTag);
    }
    else
    {
        isMoving = true;
        isMoved = false;

        auto it = std::find(bookTags.begin(), bookTags.end(), itemTag);
        if (it != bookTags.end())
            bookTags.erase(it);
    }

    printf("Solved: %d\n", Book::solved);
}

/**
 * @brief Updates the book each frame.
 *
 * Handles:
 * - Movement animation (pulling out / pushing in)
 * - Transition to solved position when puzzle is completed
 * - Collision box updates
 *
 * @param deltaTime Time elapsed since last frame.
 */
void Book::Update(float deltaTime)
{
    // Trigger solved movement once
    if (Book::solved && !isSolvedMoving)
    {
        isSolvedMoving = true;
        isMoving = false;
    }

    // Handle normal movement
    if (isMoving)
    {
        if (isMoved)
        {
            position = glm::mix(position, movedPosition, deltaTime * 2.0f);
            UpdateCollisionBox();

            if (glm::length(position - movedPosition) < 0.01f)
            {
                isMoving = false;
                CheckSolved();
            }
        }
        else
        {
            position = glm::mix(position, originalPosition, deltaTime * 2.0f);
            UpdateCollisionBox();

            if (glm::length(position - originalPosition) < 0.01f)
            {
                isMoving = false;
                CheckSolved();
            }
        }
    }

    // Handle solved movement
    if (isSolvedMoving)
    {
        position = glm::mix(position, solvedPosition, deltaTime * 0.5f);
        UpdateCollisionBox();

        if (glm::length(position - solvedPosition) < 0.01f)
        {
            isSolvedMoving = false;
        }
    }
}

/**
 * @brief Checks whether the bookshelf puzzle is solved.
 *
 * Compares the list of moved book tags with the required solution order.
 * If solved, plays the puzzle completion sound.
 */
void Book::CheckSolved()
{
    printf("Checking solved...\n");

    if (bookTags.size() == solveSize)
    {
        bool allCorrect = true;

        for (int i = 0; i < solveSize; i++)
        {
            if (std::find(bookTags.begin(), bookTags.end(), correctBooks[i]) == bookTags.end())
            {
                allCorrect = false;
                break;
            }
        }

        if (allCorrect)
        {
            Book::solved = true;

            if (itemTag == "redBook" || itemTag == "orangeBook" ||
                itemTag == "yellowBook" || itemTag == "purpleBook")
            {
                AudioManager::GetInstance().Play3DSoundEffect("bookshelf_solved", position, 1.0f);
            }
        }
    }
}

/**
 * @brief Gets the action text displayed when targeting the book.
 *
 * @return "Examine" if puzzle solved, otherwise "Pull out" or "Put back".
 */
std::string Book::GetActionText()
{
    if (Book::solved)
        return "Examine";

    return isMoved ? "Put back" : "Pull out";
}
