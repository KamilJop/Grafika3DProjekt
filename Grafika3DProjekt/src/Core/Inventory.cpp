#include "Inventory.h"

Inventory::Inventory()
{	
	Config& config = Config::getInstance();
	textRenderer = new TextRenderer(config.screenWidth,config.screenHeight);
	textRenderer->Load("Fonts/BitterPro-Medium.ttf", 12);
}

void Inventory::AddItem(const std::string& tag, const std::string& name, Texture* tex)
{
	Item newItem;
	newItem.tag = tag;
	newItem.imageTexture = tex;
	newItem.title = name;
	items.push_back(newItem);
	printf("Item with tag '%s' added to inventory.\n", tag.c_str());

}

void Inventory::RemoveItem(const std::string& tag)
{
	for (auto it = items.begin(); it != items.end(); ++it)
	{
		if (it->tag == tag)
		{
			items.erase(it);
			printf("Item with tag '%s' removed from inventory.\n", tag.c_str());
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