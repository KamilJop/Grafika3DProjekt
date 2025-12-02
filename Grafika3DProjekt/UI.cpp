#include "UI.h"


UI::UI(GLFWwindow* window)
{
	appWindow = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(appWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UI :: DrawPauseMenu() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	float screenWidth = config.screenWidth;
	float screenHeight = config.screenHeight;
	ImVec2 center = ImVec2(screenWidth * 0.5f, screenHeight * 0.5f);
	ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

	ImVec2 dummyVec = ImVec2(10.0f, 10.0f);
	float windowWidth = 400.0f;
	float windowHeight = 600.0f;
	float middleX = windowWidth * 0.5f;
	float middleY = windowHeight * 0.5f;
	ImGui::SetNextWindowSize(ImVec2(windowWidth, windowHeight), ImGuiCond_Always);

	ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove;

	ImGui::Begin(" ", NULL, windowFlags);
	ImGui::Dummy(dummyVec);
	CenteredText("Game Paused");
	ImGui::Dummy(dummyVec);
	CenteredText("Press ESC to Resume");
	ImGui::Dummy(dummyVec);
	ImGui::Separator();
	ImGui::Dummy(dummyVec);
	CenteredText("Adjust Volume:");
	ImGui::Dummy(dummyVec);

	ImGui::SliderFloat("Master Volume", &config.musicVolume, 0.0f, 1.0f);

	ImGui::SliderFloat("SFX Volume", &config.sfxVolume, 0.0f, 1.0f);

	ImGui::Dummy(dummyVec);
	ImGui::Separator();
	ImGui::Dummy(dummyVec);

	CenteredText("Graphic options");
	ImGui::Dummy(dummyVec);

	CenteredText("Resolution:");
	ImGui::Dummy(dummyVec);
	static int currentResolution = 0;

	middleX = middleX - 100.0f;
	ImGui::SetCursorPosX(middleX);
	ImGui::SetNextItemWidth(200.0f);
	ImGui::Combo("##Resolution", &currentResolution, resolutionOptions, IM_ARRAYSIZE(resolutionOptions));


	ImGui::Dummy(dummyVec);
	float textSize = ImGui::CalcTextSize("Show FPS?:").x;
	middleX = windowWidth * 0.5f - (textSize * 0.5f) - 10.0f;
	ImGui::SetCursorPosX(middleX);
	ImGui::Checkbox("Show FPS?", &config.showFPS);	

	ImGui::Dummy(dummyVec);
	CenteredText("Entity outline color");
	float pickerWidth = 300.0f;
	middleX = windowWidth * 0.5f - (pickerWidth * 0.5f);
	ImGui::SetCursorPosX(middleX);
	ImGui::SetNextItemWidth(pickerWidth);
	ImGui::ColorEdit3("##", config.outlineColor);

	ImGui::Dummy(dummyVec);
	CenteredText("Highlight text color");
	middleX = windowWidth * 0.5f - (pickerWidth * 0.5f);
	ImGui::SetCursorPosX(middleX);
	ImGui::SetNextItemWidth(pickerWidth);
	ImGui::ColorEdit3("###", config.highlightColor);


	middleX = windowWidth * 0.5f - 60.0f; 
	float buttonY = windowHeight - 50.0f;
	ImGui::SetCursorPosX(middleX);
	ImGui::SetCursorPosY(buttonY);

	ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10.0f));

	if(ImGui::Button("Quit the game?",ImVec2(120,0))) {
		ImGui::OpenPopup("Confirm Quit");
	}
	ImGui::PopStyleVar();
	DrawQuitConfirmation();

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::DrawQuitConfirmation() {

	if (!ImGui::IsPopupOpen("Confirm Quit"))
		return;
	ImGui::BeginPopupModal("Confirm Quit", NULL, ImGuiWindowFlags_AlwaysAutoResize); 
	CenteredText("Are you sure you want to quit the game?");
	CenteredText("There are no saves in our game!");


	if (ImGui::Button("Yes", ImVec2(120, 0))) {
		glfwSetWindowShouldClose(appWindow, GLFW_TRUE);
		ImGui::CloseCurrentPopup();
	}

	ImGui::SetItemDefaultFocus();

	ImGui::SameLine();
	if (ImGui::Button("No", ImVec2(120, 0))) {
		ImGui::CloseCurrentPopup();
	}

	ImGui::EndPopup();
}

void UI::CenteredText(const char* text) {
	float windowWidth = ImGui::GetWindowSize().x;
	float textWidth = ImGui::CalcTextSize(text).x;
	ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
	ImGui::Text("%s", text);
}