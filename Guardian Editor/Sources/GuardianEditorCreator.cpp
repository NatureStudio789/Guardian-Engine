#include "GuardianEditorCreator.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace GE
{
	GuardianEditorCreator::GuardianEditorCreator()
	{

	}

	GuardianEditorCreator::~GuardianEditorCreator()
	{

	}

	void GuardianEditorCreator::Initialize()
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
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.102f, 1.0f);

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

		ImGui_ImplWin32_Init(GuardianApplication::ApplicationInstance->GetCreatorWindowHandle());
		ImGui_ImplDX11_Init(
			GuardianApplication::ApplicationInstance->GetCreatorGraphicsContext()->GetGraphicsDevice().Get(),
			GuardianApplication::ApplicationInstance->GetCreatorGraphicsContext()->GetGraphicsDeviceContext().Get());
		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

		this->AddEditorToCreator(std::make_shared<GuardianProjectEditor>());
	}

	void GuardianEditorCreator::Update()
	{
		for (auto& editor : this->CreatorEditorList)
		{
			editor.second->Update();
		}
	}

	void GuardianEditorCreator::Render()
	{
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();

		this->RenderDockspace();

		for (auto& editor : this->CreatorEditorList)
		{
			editor.second->Render();
		}

		ImGui::Render();
		GuardianApplication::ApplicationInstance->GetCreatorGraphicsContext()->GetGraphicsMainFramebuffer()->ApplyFramebuffer(
			GuardianApplication::ApplicationInstance->GetCreatorGraphicsContext(), { 0.0f, 0.0f, 0.0f });
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GuardianEditorCreator::Release()
	{
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}

	void GuardianEditorCreator::AddEditorToCreator(std::shared_ptr<EUI::GuardianEditor> editor)
	{
		editor->Initialize();

		this->CreatorEditorList[editor->GetEditorName()] = editor;
	}

	void GuardianEditorCreator::RenderDockspace()
	{
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
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
		ImGui::Begin("Guardian Editor Creator", null, window_flags);

		ImGui::PopStyleVar();
		ImGui::PopStyleVar(2);

		ImGuiID dockspace_id = ImGui::GetID("Guardian Editor Creator Dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f));

		ImGui::End();
	}

	LRESULT GuardianEditorCreator::EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}
