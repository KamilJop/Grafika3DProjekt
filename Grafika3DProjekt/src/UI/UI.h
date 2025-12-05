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

private:
	GLFWwindow* appWindow;
	const char* resolutionOptions[4] = { "1280x720", "1920x1080", "2560x1440", "Fullscreen"};
	void CenteredText(const char* text);
};

