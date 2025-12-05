#pragma once

#include <string>
#include <vector>
struct Item
{
	std::string tag;
	std::string imagePath;
};
class Inventory
{
public:
	Inventory() {};
	~Inventory() {};
	void AddItem(const std::string& tag, const std::string& imagePath);
	void RemoveItem(const std::string& tag);
	bool HasItem(const std::string& tag);
private:
	std::vector<Item> items;

};

