#pragma once
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include "../Rendering/Texture.h"
#include "../Systems/TextRenderer.h"
#include "../Systems/AudioManager.h"
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
	bool HasItem(const std::string& tag);
	void DrawNotification(float deltaTime);
	void ChangeCurrentItem(int direction);
	void SetCurrentItem(int index);
	int GetCurrentItemIndex() { return currentItemIndex; }
	int GetMaxItems() { return maxItems; }
	Item* GetCurrentItem();
	std::vector<Item> GetItems() { return items; }
private:
	int currentItemIndex = 0;
	int maxItems = 9;
	std::vector<Item> items;
	TextRenderer* textRenderer;
	float notificationTimer = 0.0f;
	float alpha = 0.0f;
	std::string notificationText = "";
	std::string slotSoundPath = "Audio/eq_change.mp3";

};

