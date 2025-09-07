#include "GProjectEngine.h"

namespace GE
{
	GProjectEngine::GProjectEngine()
	{
		this->ModuleName = "Project";
	}

	GProjectEngine::GProjectEngine(const GProjectEngine& other)
	{

	}

	GProjectEngine::~GProjectEngine()
	{

	}

	void GProjectEngine::InitializeModule()
	{
		//GProject::Instance->CreateProject("Sandbox", "Sandbox\\");
		GProject::Instance->InitializeProject("Sandbox\\Sandbox.geproj");
	}

	void GProjectEngine::UpdateModule()
	{
		/*if (GetAsyncKeyState('P') && GetAsyncKeyState(VK_CONTROL))
		{
			GProjectPacker::PackProject(GProject::Instance, "TestPack");
		}
		if (GetAsyncKeyState('U') && GetAsyncKeyState(VK_CONTROL))
		{
			GAssetPacker::UnpackAsset("TestPack\\Sandbox\\Sandbox-Asset.gepack", "TestUnpack");
		}
		if (GetAsyncKeyState('Q') && GetAsyncKeyState(VK_CONTROL))
		{
			GAssetPacker::LoadAssetPackage(GAssetLoaderRegistry::GetCurrentAssetLoader(), "TestPack\\Sandbox\\Sandbox-Asset.gepack");
		}*/
	}

	void GProjectEngine::ReleaseModule()
	{

	}
}
