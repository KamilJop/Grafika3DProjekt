#include "CandlePuzzle.h"

bool CandlePuzzle::isSolved = false;

/**
 * @brief Initializes the candle puzzle.
 *
 * Hides pedestals, offsets second candle lights,
 * and loads required sound effects.
 */
CandlePuzzle::CandlePuzzle(std::vector<Candle*> candles,
                           std::vector<Candle*> candles2,
                           std::vector<Pedestal*> ped,
                           Door* doors)
    : candles(candles), secondCandles(candles2),
      doorToLock(doors), pedestals(ped)
{
    // Hide pedestals at start
    for (auto& p : pedestals) {
        p->setVisibility(false);
        p->setColissions(false);
        p->setCastsShadow(false);
        p->setInteractable(false);
    }

    // Adjust second candle light positions
    secondCandles[0]->setLightPosition(secondCandles[0]->getLightPosition() + glm::vec3(0, 0, 0.22f));
    secondCandles[1]->setLightPosition(secondCandles[1]->getLightPosition() + glm::vec3(-0.22f, 0, 0));
    secondCandles[2]->setLightPosition(secondCandles[2]->getLightPosition() + glm::vec3(-0.22f, 0, 0));
    secondCandles[3]->setLightPosition(secondCandles[3]->getLightPosition() + glm::vec3(0, 0, -0.22f));

    // Load sounds
    AudioManager::GetInstance().Load3DSoundEffect(demonicWhisperSoundPath, demonicWhisperSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect(doorSlamSoundPath, doorSlamSoundPath);
    AudioManager::GetInstance().Load3DSoundEffect(whooshSoundPath, whooshSoundPath);
}

/**
 * @brief Updates puzzle logic.
 *
 * Handles:
 * - Candle order checking
 * - Puzzle completion
 * - Lighting second candles in sequence
 * - Playing sound effects
 */
void CandlePuzzle::Update(float dt)
{
    if (timer >= 6.0f) return;

    // Puzzle already solved → play sequence
    if (isSolved) {
        timer += dt;

        // Light candles one by one with sound
        if (timer >= 1.0f) {
            secondCandles[0]->lightUp();
            if (!whooshSoundPlayed[0]) {
                whooshSoundPlayed[0] = true;
                AudioManager::GetInstance().Play3DSoundEffect(
                    whooshSoundPath, secondCandles[0]->getPosition(),
                    Config::getInstance().sfxVolume * 10.0f
                );
            }
        }

        if (timer >= 2.0f) {
            secondCandles[1]->lightUp();
            if (!whooshSoundPlayed[1]) {
                whooshSoundPlayed[1] = true;
                AudioManager::GetInstance().Play3DSoundEffect(
                    whooshSoundPath, secondCandles[1]->getPosition(),
                    Config::getInstance().sfxVolume * 10.0f
                );
            }
        }

        if (timer >= 3.0f) {
            secondCandles[2]->lightUp();
            if (!whooshSoundPlayed[2]) {
                whooshSoundPlayed[2] = true;
                AudioManager::GetInstance().Play3DSoundEffect(
                    whooshSoundPath, secondCandles[2]->getPosition(),
                    Config::getInstance().sfxVolume * 10.0f
                );
            }
        }

        if (timer >= 4.0f) {
            secondCandles[3]->lightUp();
            if (!whooshSoundPlayed[3]) {
                whooshSoundPlayed[3] = true;
                AudioManager::GetInstance().Play3DSoundEffect(
                    whooshSoundPath, secondCandles[3]->getPosition(),
                    Config::getInstance().sfxVolume * 10.0f
                );
            }

            // Lock door once sequence finishes
            if (!doorSoundPlayed) {
                doorSoundPlayed = true;
                AudioManager::GetInstance().Play3DSoundEffect(
                    doorSlamSoundPath, doorToLock->getPosition(),
                    Config::getInstance().sfxVolume * 5.0f
                );
                doorToLock->setLocked(true);
                doorToLock->setIsOpen(false);
                doorToLock->setExamineText("Demon locked the door. I need to find a way to escape from here.");
                doorToLock->setTriedToOpen(false);
            }
        }
        return;
    }

    // Puzzle not solved → check candle order
    for (auto& candle : candles) {
        if (candle->getIsLit()) {
            std::string tag = candle->getTag();
            if (std::find(solution.begin(), solution.end(), tag) == solution.end())
                solution.push_back(tag);
        }

        // Once 5 candles lit → check order
        if (solution.size() == 5) {
            bool solved = true;
            for (size_t i = 0; i < correctOrder.size(); i++) {
                if (solution[i] != correctOrder[i]) {
                    solved = false;
                    break;
                }
            }

            if (solved) {
                isSolved = true;
                UI::SetSubtitle("GOOD.", 3.0f);

                AudioManager::GetInstance().Play3DSoundEffect(
                    demonicWhisperSoundPath, glm::vec3(17, 0, -3),
                    Config::getInstance().sfxVolume * 5.0f
                );

                // Reveal pedestals
                for (auto& p : pedestals) {
                    p->setVisibility(true);
                    p->setColissions(true);
                    p->setCastsShadow(true);
                    p->setInteractable(true);
                }

                // Blow out candles
                for (auto& candle : candles)
                    candle->blowOut();
            }
            else {
                solution.clear();
                UI::SetSubtitle("Wrong order. The candles extinguish.", 3.0f);

                for (auto& candle : candles)
                    candle->blowOut();
            }
        }
    }
}
