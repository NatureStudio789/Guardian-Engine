#include "../../GuardianEngine.h"

extern guardian::GuardianProgram* CreateProgram();
#define GUARDIAN_ENGINE_PROFILE(Program) guardian::GuardianProgram* CreateProgram() { return new Program();  }

int main()
{
	try
	{
		guardian::GuardianEngine::EngineInstance->InitializeEngine(CreateProgram());

		guardian::GuardianEngine::EngineInstance->LaunchEngine();

		return guardian::GE_SUCCESS;
	}
	catch (const guardian::GuardianException& e)
	{
		MessageBoxA(null, e.what(), "Guardian Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception& e)
	{
		MessageBoxA(null, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(null, "No detail description", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
	}

	return guardian::GE_FAILURE;
}
