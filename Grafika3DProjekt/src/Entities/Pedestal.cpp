#include "Pedestal.h"

Pedestal::Pedestal(Model* model, glm::vec3 pos, glm::vec3 rot, glm::vec3 scal,Scene* scene, PointLight* pl,bool interaction)
	: Entity(model, pos, rot, scal, interaction)
{
	pedestalEntity = nullptr;
	storedItem = nullptr;
	currentScene = scene;
	pedestalLight = pl;
	pedestalLight->setPosition(pos + glm::vec3(0.0f, 2.0f, 0.0f));
	pedestalLight->setAmbientIntensity(0.0f);
	pedestalLight->setDiffuseIntensity(0.0f);
	pedestalLight->setColor(glm::vec3(0.0f));
}


void Pedestal::Interact(Inventory* playerInventory)
{
	if (isItemPlaced) {
		isItemPlaced = false;
		playerInventory->AddItem(storedItem->tag, storedItem->title, storedItem->imageTexture, storedItem->itemModel, storedItem->itemScale);
		pedestalEntity->setShouldGetDestroyed(true);
		pedestalEntity = nullptr;
		storedItem = nullptr;
		hasCorrectItem = false;
		return;
	}
	if (playerInventory->GetCurrentItem() == nullptr) return;
	Item* currentItem = playerInventory->GetCurrentItem();
	if (currentItem->tag.length() <= 7) return;
	if(currentItem->tag.substr(0,8) == "pedestal") {
		Model* itemModel = currentItem->itemModel;
		glm::vec3 itemScale = currentItem->itemScale;
		pedestalEntity = new Entity(itemModel, getPosition() + glm::vec3(0.0f, 1.3f, 0.0f), rotation, itemScale, false);
		currentScene->AddEntity(pedestalEntity);
		storedItem = new Item{
			currentItem->tag,
			currentItem->title,
			currentItem->imageTexture,
			currentItem->itemModel,
			currentItem->itemScale
		};
		playerInventory->RemoveItem(storedItem->tag);
		isItemPlaced = true;
	}
}


void Pedestal::Update(float deltaTime)
{
	if (isVisible) {
		if (hasCorrectItem) {
			pedestalLight->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
			pedestalLight->setDiffuseIntensity(1.0f);
			pedestalLight->setAmbientIntensity(0.5f);
		}
		else {
			pedestalLight->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
			pedestalLight->setDiffuseIntensity(1.0f);
			pedestalLight->setAmbientIntensity(0.5f);
		}
	}
	if(pedestalEntity != nullptr) {
		pedestalEntity->setRotation(pedestalEntity->getRotation() + glm::vec3(0.0f,30.0f * deltaTime, 0.0f));

		if (storedItem->tag == correctItemTag) {
			hasCorrectItem = true;
		}
		else {
			hasCorrectItem = false;
		}

	}
}