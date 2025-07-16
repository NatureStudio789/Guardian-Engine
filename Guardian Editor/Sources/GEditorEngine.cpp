#include "GEditorEngine.h"

namespace GE
{
    GEditorEngine::GEditorEngine()
    {
        
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

        this->ProgramAttribute.ProgramRequiredModuleList = { "Render", "Audio" };
    }

    void GEditorEngine::InitializeProgram()
    {

    }

    void GEditorEngine::UpdateProgram()
    {

    }

    void GEditorEngine::ReleaseProgram()
    {

    }
}
