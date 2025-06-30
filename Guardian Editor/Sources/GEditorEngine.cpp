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
