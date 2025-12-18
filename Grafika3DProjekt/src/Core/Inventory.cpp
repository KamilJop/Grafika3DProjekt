#include "Inventory.h"
Inventory::Inventory()
{	
	Config& config = Config::getInstance();
	textRenderer = new TextRenderer(config.screenWidth,config.screenHeight);
	textRenderer->Load("Fonts/BitterPro-Medium.ttf", 16);
	AudioManager::GetInstance().Load2DSoundEffect("slot_change", slotSoundPath);
}

void Inventory::AddItem(const std::string& tag, const std::string& name, Texture* tex, Model* model, glm::vec3 scale)
{

	Item newItem;
	newItem.tag = tag;
	newItem.imageTexture = tex;
	newItem.title = name;
	newItem.itemModel = model;
	newItem.itemScale = scale;
	items.push_back(newItem);
	printf("Item with tag '%s' added to inventory.\n", tag.c_str());
	notificationText = "Item picked up:    " + name;
	notificationTimer = 2.0f; 
	alpha = 1.0f;

}

void Inventory::RemoveItem(const std::string& tag)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		if (it->tag == tag)
		{

			printf("Item with tag '%s' removed from inventory.\n", tag.c_str());
			notificationText = "Item removed:    " + it->title;
			notificationTimer = 2.0f;
			alpha = 1.0f;
			items.erase(it);
			return;
		}
	}
}

bool Inventory::HasItem(const std::string& tag)
{
	for (const auto& item : items)
	{
		if (item.tag == tag)
		{
			return true;
		}
	}
	return false;
}

void Inventory::DrawNotification(float deltaTime)
{
	if (notificationTimer >= 0.0f)
	{
		notificationTimer -= deltaTime;
		alpha -= deltaTime * 0.5f;
		float offsetY = 240.0f;
		float offsetX = 40.0f;
		float textWidth = textRenderer->GetTextWidth(notificationText);

		textRenderer->RenderText(notificationText, offsetX, offsetY, 1.0 , glm::vec4(1.0f, 1.0f, 1.0f, alpha));
	}
	
}


void Inventory::ChangeCurrentItem(int direction)
{
	AudioManager::GetInstance().Play2DSoundEffect("slot_change", Config::getInstance().sfxVolume);
	if(direction > 0) {
		currentItemIndex++;
		if(currentItemIndex >= maxItems) {
			currentItemIndex = 0;
		}
	} else if(direction < 0) {
		currentItemIndex--;
		if(currentItemIndex < 0) {
			currentItemIndex = maxItems - 1;
		}
	}
}

void Inventory::SetCurrentItem(int index)
{
	AudioManager::GetInstance().Play2DSoundEffect("slot_change", Config::getInstance().sfxVolume);
	if (index >= 0 && index < maxItems) {
		currentItemIndex = index;
	}
}

Item* Inventory::GetCurrentItem()
{
	Item* currentItem = nullptr;
	if (items.size() > 0 && currentItemIndex >= 0 && currentItemIndex < items.size()) {
		currentItem = &items[currentItemIndex];
	}
	else {
		currentItem = &emptyItem;
	}
	return currentItem;
}

