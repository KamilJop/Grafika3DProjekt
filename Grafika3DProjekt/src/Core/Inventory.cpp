#include "Inventory.h"
#include "Config.h"
#include <cstdio>

/**
 * @brief Constructs an Inventory instance.
 * 
 * Initializes the text renderer and loads the slot change sound.
 */
Inventory::Inventory()
{
    Config& config = Config::getInstance();
    textRenderer = new TextRenderer(config.screenWidth, config.screenHeight);
    textRenderer->Load("Fonts/BitterPro-Medium.ttf", 16);

    AudioManager::GetInstance().Load2DSoundEffect("slot_change", slotSoundPath);
}

/**
 * @brief Adds a new item to the inventory.
 * 
 * @param tag Unique identifier for the item.
 * @param name Display name of the item.
 * @param tex Pointer to the item's texture.
 * @param model Pointer to the item's 3D model.
 * @param scale Scale to render the 3D model.
 * 
 * Sets up a notification text when the item is picked up.
 */
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

/**
 * @brief Removes an item from the inventory by tag.
 * 
 * @param tag The unique identifier of the item to remove.
 * 
 * Updates notification text when an item is removed.
 */
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

/**
 * @brief Checks if an item exists in the inventory.
 * 
 * @param tag The unique identifier of the item.
 * @return true If the item exists.
 * @return false Otherwise.
 */
bool Inventory::HasItem(const std::string& tag)
{
    for (const auto& item : items)
    {
        if (item.tag == tag)
            return true;
    }
    return false;
}

/**
 * @brief Renders the notification text for recently added/removed items.
 * 
 * @param deltaTime Time elapsed since last frame (used for fading effect).
 */
void Inventory::DrawNotification(float deltaTime)
{
    if (notificationTimer >= 0.0f)
    {
        notificationTimer -= deltaTime;
        alpha -= deltaTime * 0.5f;

        float offsetY = 240.0f;
        float offsetX = 40.0f;

        textRenderer->RenderText(notificationText, offsetX, offsetY, 1.0, glm::vec4(1.0f, 1.0f, 1.0f, alpha));
    }
}

/**
 * @brief Changes the currently selected item in the inventory.
 * 
 * @param direction +1 for next item, -1 for previous item.
 * 
 * Plays a slot change sound effect.
 */
void Inventory::ChangeCurrentItem(int direction)
{
    AudioManager::GetInstance().Play2DSoundEffect("slot_change", Config::getInstance().sfxVolume);

    if (direction > 0)
    {
        currentItemIndex++;
        if (currentItemIndex >= maxItems)
            currentItemIndex = 0;
    }
    else if (direction < 0)
    {
        currentItemIndex--;
        if (currentItemIndex < 0)
            currentItemIndex = maxItems - 1;
    }
}

/**
 * @brief Sets the current item by index.
 * 
 * @param index Index of the item to select.
 */
void Inventory::SetCurrentItem(int index)
{
    AudioManager::GetInstance().Play2DSoundEffect("slot_change", Config::getInstance().sfxVolume);

    if (index >= 0 && index < maxItems)
        currentItemIndex = index;
}

/**
 * @brief Returns a pointer to the currently selected item.
 * 
 * @return Item* Pointer to the current item, or an empty item if inventory is empty.
 */
Item* Inventory::GetCurrentItem()
{
    Item* currentItem = nullptr;

    if (items.size() > 0 && currentItemIndex >= 0 && currentItemIndex < items.size())
    {
        currentItem = &items[currentItemIndex];
    }
    else
    {
        currentItem = &emptyItem;
    }

    return currentItem;
}
