#pragma once
#include "Entity.h"
#include "../Systems/AudioManager.h"
#include <string>
#include "../Rendering/Texture.h"
class Book : public Entity
{
public:
	Book(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, bool interaction = true);
	void Interact(Inventory* playerInventory) override;
	void Update(float deltaTime) override;
	~Book();
	static std::vector<std::string> bookTags;
	static bool solved;
private:
	bool isMoved = false;
	bool isMoving = false;
	bool isSolvedMoving = false;
	void CheckSolved();
	std::string bookshelfSolvedSoundPath = "Audio/bookshelf_solved.mp3";
	std::vector<std::string>movingSoundPaths = {
		"Audio/book_move1.mp3",
		"Audio/book_move2.mp3",
		"Audio/book_move3.mp3",
		"Audio/book_move4.mp3"
	};
	glm::vec3 originalPosition;
	glm::vec3 movedPosition;
	glm::vec3 solvedPosition;
	int solveSize = 4;
	std::string correctBooks[4] = { "redBook", "greenBook", "blueBook", "yellowBook" };
};

