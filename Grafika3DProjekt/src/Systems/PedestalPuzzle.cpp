#include "PedestalPuzzle.h"

/**
 * @brief Creates the pedestal puzzle.
 *
 * Loads the sound that plays when the puzzle is solved.
 *
 * @param pedestal List of pedestals involved in the puzzle.
 * @param door1 First door to unlock when solved.
 * @param door2 Second door to unlock when solved.
 */
PedestalPuzzle::PedestalPuzzle(std::vector<Pedestal*> pedestal,
                               Door* door1, Door* door2)
    : pedestals(pedestal), doorToUnlock1(door1), doorToUnlock2(door2)
{
    AudioManager::GetInstance().Load3DSoundEffect(
        puzzleSolvedSoundPath, puzzleSolvedSoundPath
    );
}

/**
 * @brief Updates the puzzle state.
 *
 * Checks whether all pedestals have the correct item.
 * If solved:
 * - Unlocks both doors
 * - Opens both doors
 * - Plays the puzzle‑solved sound once
 */
void PedestalPuzzle::Update()
{
    bool allCorrect = true;

    // Check if every pedestal has the correct item
    for (auto& pedestal : pedestals) {
        if (!pedestal->getHasCorrectItem()) {
            allCorrect = false;
            break;
        }
    }

    // Puzzle solved
    if (allCorrect) {
        doorToUnlock1->setLocked(false);
        doorToUnlock2->setLocked(false);
        doorToUnlock1->setIsOpen(true);
        doorToUnlock2->setIsOpen(true);

        puzzleSolved = true;

        // Play solved sound once
        if (!puzzleSolvedPlayed) {
            puzzleSolvedPlayed = true;
            AudioManager::GetInstance().Play3DSoundEffect(
                puzzleSolvedSoundPath,
                glm::vec3(0.0f),
                Config::getInstance().sfxVolume * 2.0f
            );
        }
    }
}
