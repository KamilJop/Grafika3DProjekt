#include "Book.h"

std::vector<std::string> Book::bookTags = {};
bool Book::solved = false;

Book::Book(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, std::string tag, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	itemTag = tag;
	pickable = false;
	originalPosition = pos;
	movedPosition = pos + glm::vec3(0.0f, 0.0f, 0.23f);
	solvedPosition = pos + glm::vec3(-2.0f, 0.0f, 0.0f);
	if (itemTag == "redBook" || itemTag == "blueBook" || itemTag == "yellowBook" || itemTag =="greenBook") {
		AudioManager::GetInstance().Load3DSoundEffect("bookshelf_solved", bookshelfSolvedSoundPath);
	}
	for(int i = 0; i < movingSoundPaths.size(); i++) {
		AudioManager::GetInstance().Load3DSoundEffect("book_move_" + std::to_string(i), movingSoundPaths[i]);
	}
}

void Book::Interact(Inventory* playerInventory)
{
	int soundIndex = rand() % movingSoundPaths.size();
	if (Book::solved) return;
	AudioManager::GetInstance().Play3DSoundEffect("book_move_" + std::to_string(soundIndex), position, 1.0f);
	if (!isMoved) {
		isMoving = true;
		isMoved = true;
		bookTags.push_back(itemTag);
	}
	else {
		isMoving = true;
		isMoved = false;
		auto it = std::find(bookTags.begin(), bookTags.end(), itemTag);
		if (it != bookTags.end()) {
			bookTags.erase(it);
		}
	}
	printf("Solved: %d\n", Book::solved);
}


void Book::Update(float deltaTime)
{
	if (Book::solved && !isSolvedMoving) {
		isSolvedMoving = true;
		isMoving = false;
	}
	if (isMoving){
		if(isMoved){
			position = glm::mix(position, movedPosition, deltaTime * 2.0f);
			UpdateCollisionBox();
			if(glm::length(position - movedPosition) < 0.01f) {
				isMoving = false;
				CheckSolved();
			}
		}
		else {
			position = glm::mix(position, originalPosition, deltaTime * 2.0f);
			UpdateCollisionBox();
			if (glm::length(position - originalPosition) < 0.01f) {
				isMoving = false;
				CheckSolved();

			}
		}
	}
	if (isSolvedMoving) {
		position = glm::mix(position, solvedPosition, deltaTime * 0.5f);
		UpdateCollisionBox();
		if (glm::length(position - solvedPosition) < 0.01f) {
			isSolvedMoving = false;
		}
	}
}




void Book::CheckSolved()
{
	printf("Checking solved...\n");
	if (bookTags.size() == solveSize) {
		bool allCorrect = true;
		for (int i = 0; i < solveSize; i++) {
			if (std::find(bookTags.begin(), bookTags.end(), correctBooks[i]) == bookTags.end()) {
				allCorrect = false;
				break;
			}
		}
		if (allCorrect) {
			Book::solved = true;
			if (itemTag == "redBook" || itemTag == "blueBook" || itemTag == "yellowBook" || itemTag == "greenBook") {
				AudioManager::GetInstance().Play3DSoundEffect("bookshelf_solved", position, 1.0f);
			}
		}
	}
}