#pragma once

#include <string>
#include <vector>
#include "../Rendering/Texture.h"
struct Item
{
	std::string tag;
	Texture* imageTexture;
};
class Inventory
{
public:
	Inventory() {};
	~Inventory() {};
	void AddItem(const std::string& tag, Texture* tex);
	void RemoveItem(const std::string& tag);
	bool HasItem(const std::string& tag);
	std::vector<Item> GetItems() { return items; }
private:
	std::vector<Item> items;

};

