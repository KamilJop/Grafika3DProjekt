#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Core/Config.h"
#include "../Systems/AudioManager.h"


#include "glad/glad.h"
#include <GLFW/glfw3.h>
class UI
{
public:
	Config& config = Config::getInstance();
	UI(GLFWwindow* window);
	~UI();
	void DrawPauseMenu();
	void DrawQuitConfirmation();
	void DrawResolutionConfirmation();

private:
	GLFWwindow* appWindow;
	int currentResolution;
	const char* resolutionOptions[3] = { "1280x720", "1920x1080", "2560x1440"};
	const int resolutionValues[3][2] = { {1280,720}, {1920,1080}, {2560,1440} };
	void CenteredText(const char* text);
};

