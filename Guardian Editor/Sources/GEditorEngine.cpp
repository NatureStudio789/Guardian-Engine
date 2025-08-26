#include "GEditorEngine.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace GE
{
    GEditorEngine::GEditorEngine()
    {
        this->EditorContext = null;
    }

    GEditorEngine::~GEditorEngine()
    {
        this->ReleaseProgram();
    }

    void GEditorEngine::PreInitializeProgram()
    {
		this->ProgramAttribute.ProgramName = "Guardian Engine";

		this->ProgramAttribute.ProgramWindowAttribute.WindowTitle = "Guardian Engine";
		this->ProgramAttribute.ProgramWindowAttribute.WindowStyle = GWindow::GE_STYLE_DEFAULTWINDOW;
		this->ProgramAttribute.ProgramWindowAttribute.WindowTheme = GWindow::GE_THEME_DARK;
		this->ProgramAttribute.ProgramWindowAttribute.SetCustomizedTitleBarEnable(false);

        this->ProgramAttribute.ProgramRequiredModuleList = { "Render", "Asset", "Project", "Audio", "Gameplay"};
    }

    void GEditorEngine::InitializeProgram()
    {
        this->EditorAssetLoader = std::make_shared<GAssetLoader>("EditorAssetLoader", "..\\Guardian Engine\\Resources\\Editor\\");
        GAssetLoaderRegistry::RegistryAssetLoader(this->EditorAssetLoader);

        GRenderer::RegisterRenderGraph(std::make_shared<GEditorRenderGraph>("Editor"));
    }

    void GEditorEngine::UpdateProgram()
    {

    }

    void GEditorEngine::ReleaseProgram()
    {
        this->EditorContext.reset();
        this->EditorContext = null;
    }

    LRESULT GEditorEngine::ProcessWindowMessage(GWindow::Handle handle, UINT message, WPARAM wParam, LPARAM lParam)
    {
        return ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam);
    }
}
