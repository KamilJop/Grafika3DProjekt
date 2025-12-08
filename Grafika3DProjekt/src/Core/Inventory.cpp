#include "Inventory.h"


void Inventory::AddItem(const std::string& tag, Texture* tex)
{
	Item newItem;
	newItem.tag = tag;
	newItem.imageTexture = tex;
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