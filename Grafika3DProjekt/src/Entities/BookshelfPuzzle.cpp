#include "BookshelfPuzzle.h"

BookshelfPuzzle::BookshelfPuzzle(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal, bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	pickable = false;
	originalPosition = pos;
	solvedPosition = pos + glm::vec3(-2.0f, 0.0f, 0.0f);
}

void BookshelfPuzzle::Interact(Inventory* playerInventory)
{
	if (isMoving) return;
	isMoving = true;
}


void BookshelfPuzzle::Update(float deltaTime)
{
	if (ended) return;

	if(Book::solved && !isMoving) {
		isMoving = true;
	}
	if (!isMoving) return;
	position = glm::mix(position, solvedPosition, deltaTime * 0.5f);
	UpdateCollisionBox();
	if (glm::length(position - solvedPosition) < 0.01f) {
		isMoving = false;
		ended = true;
	}
}

BookshelfPuzzle::~BookshelfPuzzle()
{
	isMoving = false;
}