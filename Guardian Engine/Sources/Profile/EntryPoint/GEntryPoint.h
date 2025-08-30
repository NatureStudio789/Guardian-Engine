#ifndef _GE_GENTRYPOINT_H_
#define _GE_GENTRYPOINT_H_
#include "../../GEngine.h"

#define GUARDIAN_SUBMIT_PROGRAM(ProgramClass)\
	std::shared_ptr<GE::GProgram> CreateProgram()\
	{\
		return std::make_shared<ProgramClass>();\
	}\

extern std::shared_ptr<GE::GProgram> CreateProgram();

#ifdef GE_RELEASE
int WINAPI WinMain
(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
#else
int main(int argc, char* argv[])
#endif
{
	try
	{
		GE::GEngine::Instance->InitializeEngine(CreateProgram());

		GE::GEngine::Instance->LaunchEngine();

		GE::GEngine::Instance->ReleaseEngine();

		return 0;
	}
	catch (const GE::GException& e)
	{
		::MessageBoxA(null, e.what(), "Guardian Exception - ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	catch (const std::exception& e)
	{
		::MessageBoxA(null, e.what(), "Standard Exception - ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
	catch (...)
	{
		::MessageBoxA(null, "For an unknown cause.", "Unknown Exception - ERROR", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}
}

#endif