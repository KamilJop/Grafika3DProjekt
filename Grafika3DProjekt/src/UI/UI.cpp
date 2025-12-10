#include "UI.h"


UI::UI(GLFWwindow* window)
{
	appWindow = window;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   

	switch (config.screenWidth) {
		case 1280:
			currentResolution = 0;
			break;
		case 1920:
			currentResolution = 1;
			break;
		case 2560:
			currentResolution = 2;
			break;
	}

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(appWindow, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void UI :: DrawPauseMenu() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();


	const ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImVec2 viewportCenter = viewport->GetCenter();
	ImGui::SetNextWindowPos(viewportCenter, ImGuiCond_Always, ImVec2(0.5f, 0.5f));

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

	middleX = middleX - 100.0f;
	ImGui::SetCursorPosX(middleX);
	ImGui::SetNextItemWidth(200.0f);
	if (ImGui::Combo("##Resolution", &currentResolution, resolutionOptions, IM_ARRAYSIZE(resolutionOptions))) {
		ImGui::OpenPopup("Confirm Resolution Change");
	};

	ImGui::Dummy(dummyVec);
	float textSize = ImGui::CalcTextSize("Show FPS?:").x;
	middleX = windowWidth * 0.5f - (textSize * 0.5f) - 10.0f;
	ImGui::SetCursorPosX(middleX);
	ImGui::Checkbox("Show FPS?", &config.showFPS);	

	ImGui::Dummy(dummyVec);
	textSize = ImGui::CalcTextSize("Fullscreen?:").x;
	middleX = windowWidth * 0.5f - (textSize * 0.5f) - 10.0f;
	ImGui::SetCursorPosX(middleX);
	if (ImGui::Checkbox("Fullscreen?", &config.fullscreen)) {
		ImGui::OpenPopup("Confirm Resolution Change");
	};

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
	DrawResolutionConfirmation();

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
		config.Save();
		AudioManager::GetInstance().Cleanup();
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

void UI::DrawResolutionConfirmation() {
	if (!ImGui::IsPopupOpen("Confirm Resolution Change"))
		return;
	ImGui::BeginPopupModal("Confirm Resolution Change", NULL, ImGuiWindowFlags_AlwaysAutoResize);
	CenteredText("Those changes need restart of the game");
	if (ImGui::Button("Restart Now", ImVec2(120, 0))) {
		AudioManager::GetInstance().Cleanup();
		config.screenWidth = resolutionValues[currentResolution][0];
		config.screenHeight = resolutionValues[currentResolution][1];
		config.Save();
		glfwSetWindowShouldClose(appWindow, GLFW_TRUE);
		ImGui::CloseCurrentPopup();
	}
	ImGui::SetItemDefaultFocus();
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(120, 0))) {
		currentResolution = 0;
		switch (config.screenWidth) {
		case 1280:
			currentResolution = 0;
			break;
		case 1920:
			currentResolution = 1;
			break;
		case 2560:
			currentResolution = 2;
			break;
		}
		config.fullscreen = !config.fullscreen;
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