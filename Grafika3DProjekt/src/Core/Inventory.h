#pragma once

#include <string>
#include <vector>
#include "../Rendering/Texture.h"
#include "../Systems/TextRenderer.h"
#include "Config.h"
struct Item
{
	std::string tag;
	std::string title;
	Texture* imageTexture;
};
class Inventory
{
public:
	Inventory();
	~Inventory() {};
	void AddItem(const std::string& tag,const std::string& name, Texture* tex);
	void RemoveItem(const std::string& tag);
	bool HasItem(const std::string& tag);\
	void DrawNotification(float deltaTime);
	std::vector<Item> GetItems() { return items; }
private:
	std::vector<Item> items;
	TextRenderer* textRenderer;
	float notificationTimer = 0.0f;
	float alpha = 0.0f;
	std::string notificationText = "";

};

