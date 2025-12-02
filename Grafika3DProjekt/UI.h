#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "glad/glad.h"

class UI
{
public:
	UI(GLFWwindow* window);
	~UI();
	void DrawPauseMenu();

private:
	GLFWwindow* appWindow;
};

