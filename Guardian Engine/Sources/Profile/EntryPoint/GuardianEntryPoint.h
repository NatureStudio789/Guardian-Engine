#include "../../GuardianEngine.h"

extern GE::GuardianProgram* CreateProgram();
#define GUARDIAN_ENGINE_PROFILE(Program) GE::GuardianProgram* CreateProgram() { return new Program();  }

int main(int argc, char* argv[])
{
	try
	{
		GE::GuardianApplication::SetCommandLineArgument(argc, argv);
		GE::GuardianEngine::EngineInstance->InitializeEngine(CreateProgram());

		GE::GuardianEngine::EngineInstance->LaunchEngine();

		return GE::GE_SUCCESS;
	}
	catch (const GE::GuardianException& e)
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

	return GE::GE_FAILURE;
}
