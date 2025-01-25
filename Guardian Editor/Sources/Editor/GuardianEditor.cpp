#include "GuardianEditor.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace guardian
{
	GuardianEditor::GuardianEditor()
	{
		this->ProgramName = "Guardian Engine - <Nature Software>";
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

		ImGui::StyleColorsDark();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 1.0f);
			style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
			style.Colors[ImGuiCol_TitleBg] = ImVec4(0.2f, 0.2f, 0.2f, 1.0f);
			style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		ImGui_ImplWin32_Init(GuardianApplication::ApplicationInstance->GetApplicationWindowHandle());
		ImGui_ImplDX11_Init(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->GetGraphicsDevice().Get(),
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext()->GetGraphicsDeviceContext().Get());

		io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);

		Nanosuit = GuardianModel::CreateNewModel(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), "Resources/Models/Nanosuit/Nanosuit.obj");
		Sponza = GuardianModel::CreateNewModel(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), "Resources/Models/Sponza/sponza.obj");
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

		ImGui::Begin("Properties");

		static float degreesX = 0.0f;
		static float degreesY = 0.0f;
		static float degreesZ = 0.0f;
		ImGui::SliderFloat("X Angle", &degreesX, -360.0f, 360.0f);
		ImGui::SliderFloat("Y Angle", &degreesY, -360.0f, 360.0f);
		ImGui::SliderFloat("Z Angle", &degreesZ, -360.0f, 360.0f);

		static float X = 0.0f;
		static float Y = 0.0f;
		static float Z = 0.0f;
		ImGui::SliderFloat("X", &X, -50.0f, 50.0f);
		ImGui::SliderFloat("Y", &Y, -50.0f, 50.0f);
		ImGui::SliderFloat("Z", &Z, -50.0f, 50.0f);
		Nanosuit->UpdateModel(GuardianTransform(GVector3(X * 0.1f, Y * 0.1f, Z * 0.1f),
			GVector3(degreesX * 0.01f, degreesY * 0.01f, degreesZ * 0.01f), GVector4(),
			GVector3(50.0f, 50.0f, 50.0f)).GetTransformMatrix());
		Sponza->UpdateModel(XMMatrixIdentity());

		ImGui::End();

		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}

	void GuardianEditor::Release()
	{
		
	}

	void GuardianEditor::RenderDockspace()
	{
		static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (DockspaceFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			WindowFlags |= ImGuiWindowFlags_NoBackground;

		static bool open = true;
		if (open)
		{
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::Begin("Guardian Engine Editor", &open, WindowFlags);
			ImGui::PopStyleVar();

			ImGui::PopStyleVar(2);

			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID DockspaceId = ImGui::GetID("Guardian Dockspace");
				ImGui::DockSpace(DockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
			}

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File(F)"))
				{


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edite(E)"))
				{


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Viewport(V)"))
				{


					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Project(P)"))
				{


					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::End();
		}
	}

	LRESULT GuardianEditor::EditorMessageProcess(GWindowHandle hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		return ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam);
	}
}
