#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Config.h"
#include "../Systems/AudioManager.h"
#include "../Systems/TextRenderer.h"


#include "glad/glad.h"
#include <GLFW/glfw3.h>

class UI
{
public:
	Config& config = Config::getInstance();
	UI(GLFWwindow* window, TextRenderer* textRenderer);
	~UI();
	void DrawPauseMenu();
	void DrawQuitConfirmation();
	void DrawResolutionConfirmation();
	static void SetSubtitle(const std::string& subtitle, float duration);
	void RenderSubtitle(float deltaTime);
	static bool wasPauseMenuOpen;
	static bool isPauseMenuOpen;

private:
	GLFWwindow* appWindow;
	int width;
	int height;
	int currentResolution;
	const char* resolutionOptions[3] = { "1280x720", "1920x1080", "2560x1440"};
	const int resolutionValues[3][2] = { {1280,720}, {1920,1080}, {2560,1440} };
	void CenteredText(const char* text);
	TextRenderer* subtitlesRenderer;
	static std::string currentSubtitle;
	static float subtitleTimer;
};
