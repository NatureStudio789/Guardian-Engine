#include "GuardianEditor.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace guardian
{
	GuardianEditor::GuardianEditor()
	{
		this->ProgramName = "Guardian Engine - <Nature Software>";
		this->EditorPanelList.clear();
	}

	GuardianEditor::~GuardianEditor()
	{
		this->Release();
	}

	void GuardianEditor::Initialize()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

		ImGuiStyle& style = ImGui::GetStyle();
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);

		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.08f, 0.08f, 0.08f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.2f, 0.205f, 0.201f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.3f, 0.305f, 0.301f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15f, 0.1505f, 0.1501f, 1.0f);

		style.Colors[ImGuiCol_Header] = ImVec4(0.2f, 0.205f, 0.201f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.3f, 0.305f, 0.301f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.1505f, 0.1501f, 1.0f);

		style.Colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.38f, 0.3805f, 0.381f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.28f, 0.2805f, 0.281f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.15f, 0.1505f, 0.151f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.2f, 0.205f, 0.21f, 1.0f);
		
		ImGui_ImplWin32_Init(GuardianApplication::ApplicationInstance->GetApplicationWindowHandle());
		ImGui_ImplDX11_Init(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->GetGraphicsDevice().Get(),
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->GetGraphicsDeviceContext().Get());

		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

		this->EditorScenePanel = std::make_shared<GuardianScenePanel>(GuardianEngine::EngineInstance->EngineScene.get());
		this->AddPanelToEditor(this->EditorScenePanel);

		this->EditorSceneHierarchyPanel = 
			std::make_shared<GuardianSceneHierarchyPanel>(GuardianEngine::EngineInstance->EngineScene.get());
		this->AddPanelToEditor(this->EditorSceneHierarchyPanel);

		this->AddPanelToEditor(std::make_shared<GuardianResourceBrowserPanel>());
	}

	void GuardianEditor::RenderDockspace()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_MenuBar;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Guardian Engine Editor", null, window_flags);

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		ImGuiID dockspace_id = ImGui::GetID("Guardian Editor Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", "Ctrl+N"))
				{
					GuardianEngine::EngineInstance->GetScene()->RemoveAllEntity();
					GuardianEngine::EngineInstance->GetScene()->CurrentScenePath = "";
					this->EditorSceneHierarchyPanel->SelectedEntityId = 0;
					GuardianEngine::EngineInstance->GetScene() = std::make_shared<GuardianScene>();
				}

				if (ImGui::MenuItem("Save", "Ctrl+S") || 
					(GuardianKeyboard::IsKeyPress(VK_CONTROL) && GuardianKeyboard::IsKeyPress('S')))
				{
					GuardianEngine::EngineInstance->GetScene()->SaveScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					GString filePath = GuardianFileDialog::SaveFile("Guardian Engine Scene (*.gscene)\0*.gscene\0");
					GuardianEngine::EngineInstance->GetScene()->SaveSceneAs(filePath);
				}

				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					GuardianEngine::EngineInstance->GetScene()->LoadScene(
						GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext());
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
	}

	void GuardianEditor::AddPanelToEditor(std::shared_ptr<GuardianPanel> panel)
	{
		this->EditorPanelList[panel->GetPanelName()] = panel;
	}

	void GuardianEditor::Update()
	{

	}

	void GuardianEditor::Render()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		this->RenderDockspace();

		for (auto& panel : this->EditorPanelList)
		{
			panel.second->Render();
		}
		this->EditorScenePanel->SetSelectedEntityId(this->EditorSceneHierarchyPanel->GetSelectedEntityId());
		this->EditorScenePanel->SetCurrentOperation(this->EditorSceneHierarchyPanel->GetCurrentOperation());

		ImGui::Render();
		GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->GetGraphicsGUIFramebuffer()->ApplyFramebuffer(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), { 0.0f, 0.0f, 0.0f });
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GuardianEditor::Release()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		for (auto& panel : this->EditorPanelList)
		{
			panel.second.reset();
		}
		this->EditorPanelList.clear();
	}

	LRESULT GuardianEditor::EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}
