#pragma once
#define NOMINMAX
#include <Windows.h>
#include <string>
#include <vector>
#include "../Rendering/Texture.h"
#include "../Systems/TextRenderer.h"
#include "../Systems/AudioManager.h"
#include "Config.h"
#include "../Rendering/Model.h"

/**
 * @brief Represents a single item in the inventory.
 */
struct Item
{
    std::string tag;        /**< Unique identifier for the item */
    std::string title;      /**< Display name of the item */
    Texture* imageTexture;  /**< Pointer to the item's 2D texture */
    Model* itemModel;       /**< Pointer to the item's 3D model */
    glm::vec3 itemScale;    /**< Scale for rendering the 3D model */
};

/**
 * @brief Manages the player's inventory.
 * 
 * Handles item storage, selection, notifications, and rendering.
 */
class Inventory
{
public:
    /**
     * @brief Constructs an Inventory object.
     * 
     * Initializes text renderer and audio for slot changes.
     */
    Inventory();

    /** Destructor */
    ~Inventory() {};

    /**
     * @brief Adds an item to the inventory.
     * 
     * @param tag Unique identifier for the item.
     * @param name Display name of the item.
     * @param tex Pointer to the item's texture.
     * @param model Pointer to the item's 3D model.
     * @param scale Scale for rendering the 3D model.
     */
    void AddItem(const std::string& tag, const std::string& name, Texture* tex, Model* model, glm::vec3 scale);

    /**
     * @brief Removes an item from the inventory by tag.
     * 
     * @param tag Unique identifier of the item to remove.
     */
    void RemoveItem(const std::string& tag);

    /**
     * @brief Checks whether an item exists in the inventory.
     * 
     * @param tag Unique identifier of the item.
     * @return true If the item is present.
     * @return false Otherwise.
     */
    bool HasItem(const std::string& tag);

    /**
     * @brief Draws a notification on screen for recently added/removed items.
     * 
     * @param deltaTime Time elapsed since last frame (used for fade-out effect).
     */
    void DrawNotification(float deltaTime);

    /**
     * @brief Changes the current selected item.
     * 
     * @param direction +1 for next item, -1 for previous item.
     */
    void ChangeCurrentItem(int direction);

    /**
     * @brief Sets the current selected item by index.
     * 
     * @param index Index of the item to select.
     */
    void SetCurrentItem(int index);

    /**
     * @brief Returns the index of the currently selected item.
     * 
     * @return int Current item index.
     */
    int GetCurrentItemIndex() { return currentItemIndex; }

    /**
     * @brief Returns the maximum number of inventory slots.
     * 
     * @return int Maximum slots.
     */
    int GetMaxItems() { return maxItems; }

    /**
     * @brief Returns a pointer to the currently selected item.
     * 
     * @return Item* Pointer to the selected item or an empty item if inventory is empty.
     */
    Item* GetCurrentItem();

    /**
     * @brief Returns a copy of all items in the inventory.
     * 
     * @return std::vector<Item> Vector containing all items.
     */
    std::vector<Item> GetItems() { return items; }

private:
    int currentItemIndex = 0;     /**< Index of the currently selected item */
    int maxItems = 6;             /**< Maximum number of inventory slots */
    std::vector<Item> items;      /**< Storage for all items in the inventory */
    TextRenderer* textRenderer;   /**< Text renderer for notifications */
    Item emptyItem = Item{ "empty", "Empty", nullptr, nullptr }; /**< Default empty item */
    float notificationTimer = 0.0f;  /**< Timer for notification fade-out */
    float alpha = 0.0f;               /**< Alpha for notification transparency */
    std::string notificationText = ""; /**< Current notification text */
    std::string slotSoundPath = "Audio/eq_change.mp3"; /**< Path to the slot change sound effect */
};
