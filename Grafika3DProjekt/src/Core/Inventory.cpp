#include "Inventory.h"

Inventory::Inventory()
{	
	Config& config = Config::getInstance();
	textRenderer = new TextRenderer(config.screenWidth,config.screenHeight);
	textRenderer->Load("Fonts/BitterPro-Medium.ttf", 16);
}

void Inventory::AddItem(const std::string& tag, const std::string& name, Texture* tex)
{
	Item newItem;
	newItem.tag = tag;
	newItem.imageTexture = tex;
	newItem.title = name;
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
			items.erase(it);
			printf("Item with tag '%s' removed from inventory.\n", tag.c_str());
			notificationText = "Item removed:    " + it->title;
			notificationTimer = 2.0f;
			alpha = 1.0f;
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
		float screenWidth = Config::getInstance().screenWidth;
		float offsetY = 120.0f;
		float textWidth = textRenderer->GetTextWidth(notificationText);
		float middleX = (screenWidth - textWidth) / 2.0f;

		textRenderer->RenderText(notificationText, middleX, offsetY, 1.0 , glm::vec4(1.0f, 1.0f, 1.0f, alpha));
	}
	
}