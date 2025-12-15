#pragma once
#include "../Systems/TextRenderer.h"
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
#include <vector>
#include <string>
#include "glad/glad.h"
#include "../Core/Window.h"
class MainMenu
{
public:
	MainMenu(unsigned int width, unsigned int height);
	~MainMenu();
	void Update(Window* window, float deltaTime);
	void Render();

private:

	unsigned int screenWidth, screenHeight;
	TextRenderer* textRenderer;
	Shader* spriteShader;
	Texture* backgroundTexture;
	int currentSelection = 0;
	float inputCooldown = 0.0f;
	std::vector<std::string>menuItems = {"Start Game", "Options", "Credits", "Exit Game"};
};

