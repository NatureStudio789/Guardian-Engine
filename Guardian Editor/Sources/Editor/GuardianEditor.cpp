#include "GuardianEditor.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
namespace guardian
{
	GuardianEditor::GuardianEditor()
	{
		this->ProgramName = "Guardian Engine - <Nature Software>";
		this->renderable = std::make_shared<GuardianMesh>();
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

		std::shared_ptr<GuardianVertexShader> vs = GuardianVertexShader::CreateNewVertexShader(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			"../Guardian Engine/Shaders/RenderableVertexShader.hlsl");
		this->renderable->AddApplicable(vs);

		D3D11_INPUT_ELEMENT_DESC id[] =
		{
			{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};

		std::shared_ptr<GuardianInputLayout> il = GuardianInputLayout::CreateNewInputLayout(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), vs, id, ARRAYSIZE(id));
		this->renderable->AddApplicable(il);

		this->renderable->AddApplicable(GuardianRasterizerState::CreateNewRasterizerState(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), GE_FILL_SOLID, GE_CULL_NONE));

		std::shared_ptr<GuardianTexture> te = GuardianTexture::CreateNewTexture(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GuardianSurface("Resources/Textures/Hakase.jpg"));
		this->renderable->AddApplicable(te);

		std::shared_ptr<GuardianSampler> sa = GuardianSampler::CreateNewSampler(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			GE_FILTER_MIN_MAG_MIP_LINEAR);
		this->renderable->AddApplicable(sa);

		std::shared_ptr<GuardianPixelShader> ps = GuardianPixelShader::CreateNewPixelShader(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(),
			"../Guardian Engine/Shaders/RenderablePixelShader.hlsl");
		this->renderable->AddApplicable(ps);

		struct Vertex
		{
			GVector3 Position;
			GVector4 Color;
			GVector2 TextureCoord;
		};

		std::vector<Vertex> vertices
		{
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}}, 
			{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},

			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},

			{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},
			{{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},

			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},

			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},
			{{ 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},

			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
			{{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 1.0f}},
			{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {0.0f, 0.0f}},
			{{ 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 0.0f}},
			{{ 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 0.5f}, {1.0f, 1.0f}},
		};

		std::shared_ptr<GuardianVertexBuffer> vb = GuardianVertexBuffer::CreateNewVertexBuffer(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), (void*)vertices.data(),
			(UINT)vertices.size(), (UINT)sizeof(Vertex));
		this->renderable->AddVertexBuffer(vb);

		cb = GuardianTransformConstantBuffer::CreateNewTransformConstantBuffer(
			GuardianApplication::ApplicationInstance->GetApplicationGraphicsContext(), GE_VERTEXSHADER_CONSTANTBUFFER);
		this->renderable->AddTransformConstantBuffer(cb);

		GuardianRenderer::SubmitRenderable(GE_SUBMIT_DEFAULT2D, this->renderable);
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
		ImGui::SliderFloat("X", &X, -20.0f, 20.0f);
		ImGui::SliderFloat("Y", &Y, -20.0f, 20.0f);
		ImGui::SliderFloat("Z", &Z, -20.0f, 20.0f);
		cb->UpdateData(GuardianTransform(GVector3(X * 0.1f, Y * 0.1f, Z * 0.1f), 
			GVector3(degreesX * 0.01f, degreesY * 0.01f, degreesZ * 0.01f), GVector4(),
			GVector3(1.0f, 1.0f, 1.0f)).GetTransformMatrix());

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
