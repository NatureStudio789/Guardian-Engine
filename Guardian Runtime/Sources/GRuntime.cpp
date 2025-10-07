#include "GRuntime.h"

namespace GE
{
	GRuntime::GRuntime()
	{

	}

	GRuntime::~GRuntime()
	{

	}

	void GRuntime::PreInitializeProgram()
	{
		inicpp::IniManager RuntimeConfig("Config/RuntimeConfig.ini");

		this->RuntimeName = (std::string)RuntimeConfig["Core"]["Name"];
		this->StartupSceneId = (unsigned long long)RuntimeConfig["Gameplay"]["StartupScene"];
		this->AssetPackage = (std::string)RuntimeConfig["Gameplay"]["AssetPackage"];

		this->ProgramAttribute.ProgramName = this->RuntimeName;

		this->ProgramAttribute.ProgramWindowAttribute.WindowTitle = this->RuntimeName;
		this->ProgramAttribute.ProgramWindowAttribute.WindowStyle = GWindow::GE_STYLE_DEFAULTWINDOW;
		this->ProgramAttribute.ProgramWindowAttribute.WindowTheme = GWindow::GE_THEME_DARK;

		this->ProgramAttribute.ProgramRequiredModuleList = { "Render", "Physics", "Asset"/*, "Audio"*/, "Gameplay"};
	}

	void GRuntime::InitializeProgram()
	{
		this->RuntimeAssetLoader = std::make_shared<GAssetLoader>();
		this->RuntimeAssetLoader->AssetDirectory = this->AssetPackage;
		this->RuntimeAssetLoader->AssetLoaderName = this->RuntimeName + "AssetLoader";
		GAssetPacker::LoadAssetPackage(this->RuntimeAssetLoader, GUtil::ExtendDirectory(this->RuntimeName, this->RuntimeName + "-Asset.gepack"));
		GAssetLoaderRegistry::RegistryAssetLoader(this->RuntimeAssetLoader);
		GAssetLoaderRegistry::SetCurrentAssetLoader(this->RuntimeAssetLoader->GetAssetLoaderName());

		this->RuntimeScene = std::make_shared<GScene>();
		GSceneSerializer::Deserialize(this->RuntimeScene, this->RuntimeAssetLoader->GetAsset(this->StartupSceneId)->GetAssetData<YAML::Node>());
		GSceneRegistry::RegisterScene(this->RuntimeScene);
		GSceneRegistry::SetActiveScene(this->RuntimeScene->GetSceneName());

		GSceneRegistry::GetActiveScene()->SwitchSceneState(GScene::GE_STATE_RUNTIME);

		auto& RenderGraph = std::make_shared<GRuntimeRenderGraph>("RuntimeMain");
		GRenderer::RegisterRenderGraph(RenderGraph);
		this->RuntimeRenderMission = GMission::CreateNewMission("RuntimeMain",
			GFramebuffer::CreateNewFramebuffer(GGraphicsContextRegistry::GetCurrentGraphicsContext()),
			std::make_shared<GCamera>());
		this->RuntimeRenderMission->Request();

		this->RuntimeEventProcesser = std::make_shared<GEventProcesser>();
		this->RuntimeEventProcesser->OnEvent<GWindowResizeEvent>([](const GWindowResizeEvent& e)
			{
				if (e.WindowHandle == GApplication::Instance->GetMainWindowHandle())
				{
					GRenderer::GetRenderGraph("SceneRuntime")->Resize(e.ResizeWidth, e.ResizeHeight);
					GRenderer::GetRenderGraph("RuntimeMain")->Resize(e.ResizeWidth, e.ResizeHeight);

					GSceneRegistry::GetActiveScene()->GetRuntimeCamera()->ResizeFrustum((float)e.ResizeWidth, (float)e.ResizeHeight);
				}
			});
	}

	void GRuntime::UpdateProgram()
	{

	}

	void GRuntime::ReleaseProgram()
	{

	}
}
